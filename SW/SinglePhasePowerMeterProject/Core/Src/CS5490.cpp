
#include "CS5490.h"
#include "main.h"
#include <math.h>
#include "usart.h"
#include "gpio.h"

static const float VoltageFullScale = 483.1873515;
static const float CurrentFullScale = 19.84466793;
static const float PowerFullScale = VoltageFullScale * CurrentFullScale;

CS5490::CS5490(UART_HandleTypeDef *huart) {
	uart = huart;
}
bool CS5490::UART_SetBaudrate() {
	volatile uint8_t reg[3];
	uint16_t baudrate = 14746; // 115200 baudrate value

	HAL_UART_DeInit(uart);
	uart->Init.BaudRate = 600;
	if (HAL_UART_Init(uart) != HAL_OK) {
		Error_Handler();
	}

	readRegister(CS5490_UART_CONTROL_REGISTER, (uint8_t*) reg);
	reg[1] = baudrate >> 8;
	reg[0] = baudrate;
	writeRegister(CS5490_UART_CONTROL_REGISTER, (uint8_t*) reg);

	HAL_UART_DeInit(uart);
	uart->Init.BaudRate = 115200;
	if (HAL_UART_Init(uart) != HAL_OK) {
		Error_Handler();
	}
}
bool CS5490::SetHPFilter() {
	volatile uint8_t reg[3];
	readRegister(CS5490_CONFIG_2_REGISTER, (uint8_t*) reg);

	// Enable HPF on Voltage and Current Channels
	reg[0] |= ((1 << 3) + (1 << 1)); // Set VFLT[0] and IFLT[0] to 1
	reg[0] &= ~((1 << 4) + (1 << 2)); // Set VFLT[1] and IFLT[1] to 0
	reg[2] |= (1 << 6); // Set POS to 1 (enable only positive Pavg values)

	writeRegister(CS5490_CONFIG_2_REGISTER, (uint8_t*) reg);
}
bool CS5490::sendBytes(uint8_t *data, uint32_t length) {
	HAL_UART_Transmit(uart, data, length, HAL_MAX_DELAY);
}
;
bool CS5490::readBytes(uint8_t *data, uint32_t length) {
	HAL_UART_Receive(uart, data, length, 200);

}
;

bool CS5490::selectPage(cs5490_register_t address) {
	volatile uint8_t data_to_send;
	data_to_send = ((address >> 8) & 0x3F) | CS5490_PAGE_SELECT_MASK;

	sendBytes((uint8_t*) &data_to_send, 1);

}
;
bool CS5490::writeRegister(cs5490_register_t address, uint8_t *data) {
	volatile uint8_t data_to_send[4];
	data_to_send[3] = data[2];
	data_to_send[2] = data[1];
	data_to_send[1] = data[0];
	selectPage(address);
	data_to_send[0] = (address & 0x3F) | CS5490_WRITE_REGISTER_MASK;
	sendBytes((uint8_t*) data_to_send, 4);
}
;
bool CS5490::readRegister(cs5490_register_t address, uint8_t *destination) {
	volatile uint8_t data_to_send;
	selectPage(address);
	data_to_send = (address & 0x3F) | CS5490_READ_REGISTER_MASK;
	sendBytes((uint8_t*) &data_to_send, 1);
	readBytes(destination, 3);
}
;
bool CS5490::writeInstruction(cs5490_instruction_t instruction) {
	volatile uint8_t data_to_send;
	data_to_send = (instruction & 0x3F) | CS5490_INSTRUCTION_MASK;
	sendBytes((uint8_t*) &data_to_send, 1);
}
;
float convertPositive24BitIntValue(uint8_t *value) {
	float val = 0.0;
	for (uint32_t i = 0; i < 24; i++) {
		val += (float) ((value[i / 8] & (1 << (i % 8))) >> (i % 8))
				* (1 / (pow(2, 24 - i)));
	}
	return val;
}
float convertNegative24BitIntValue(uint8_t *value) {
	float val = 0.0;
	for (uint32_t i = 0; i < 23; i++) {
		val += (float) ((value[i / 8] & (1 << (i % 8))) >> (i % 8))
				* (1 / (pow(2, 23 - i)));
	}
	if (value[2] && 128) {
		val *= (-1);
	}
	return val;
}
float floatAbs(float number) {
	if (number > 0) {
		return number;
	}
	return number * (-1);
}
bool CS5490::ReadMeasurements() {
	volatile uint8_t reg[3];
	volatile uint8_t tempData[6][3];
	readRegister(CS5490_STATUS_0_REGISTER, (uint8_t*) reg);
	reg[2] &= ~0x80;
	writeRegister(CS5490_STATUS_0_REGISTER, (uint8_t*) reg); // Clear conversion ready bit
	writeInstruction(CS5490_SINGLE_CONV);
	while (1) {
		readRegister(CS5490_STATUS_0_REGISTER, (uint8_t*) reg);
		if (reg[2] && 128) {
			break;
		}
	}
	reg[2] &= ~0x80;
	writeRegister(CS5490_STATUS_0_REGISTER, (uint8_t*) reg); // Clear conversion ready bit

	readRegister(CS5490_CURRENT_RMS_REGISTER, (uint8_t*) tempData[0]);
	readRegister(CS5490_VOLTAGE_RMS_REGISTER, (uint8_t*) tempData[1]);
	readRegister(CS5490_POWER_FACTOR_REGISTER, (uint8_t*) tempData[2]);
	readRegister(CS5490_ACTIVE_POWER_REGISTER, (uint8_t*) tempData[3]);
	readRegister(CS5490_REACTIVE_POWER_REGISTER, (uint8_t*) tempData[4]);
	readRegister(CS5490_APPARENT_POWER_REGISTER, (uint8_t*) tempData[5]);

	measurements[IRMS_INDEX] = convertPositive24BitIntValue(
			(uint8_t*) tempData[0]) * CurrentFullScale;
	measurements[URMS_INDEX] = (convertPositive24BitIntValue(
			(uint8_t*) tempData[1]) * VoltageFullScale);
	measurements[PF_INDEX] = floatAbs(
			convertNegative24BitIntValue((uint8_t*) tempData[2]) * 100); // units [%]
	measurements[PAVG_INDEX] = floatAbs(
			convertNegative24BitIntValue((uint8_t*) tempData[3])
					* PowerFullScale);
	measurements[QAVG_INDEX] = floatAbs(
			convertNegative24BitIntValue((uint8_t*) tempData[4])
					* PowerFullScale);
	measurements[S_INDEX] = floatAbs(
			convertNegative24BitIntValue((uint8_t*) tempData[5])
					* PowerFullScale);
	if (measurements[QAVG_INDEX] > measurements[S_INDEX]) {
		measurements[QAVG_INDEX] = 0;
	}
}
;

bool CS5490::CalibrateGain() {
	uint8_t gainDefaultValue[3] = { 0, 0, 64 };
	uint8_t settleTime[3] = { 208, 3, 0 };
	uint8_t settleTimeDefault[3] = { 30, 0, 0 };
	writeRegister(CS5490_GAIN_FOR_CURRENT, gainDefaultValue);
	writeRegister(CS5490_GAIN_FOR_VOLTAGE, gainDefaultValue);
	writeRegister(CS5490_SETTLING_TIME_FOR_CONV, settleTime);
	writeInstruction(CS5490_CALIBRATION_GAIN_I_U);
	writeRegister(CS5490_SETTLING_TIME_FOR_CONV, settleTimeDefault);
	return true;
}
bool CS5490::CalibrateOffset() {
	uint8_t gainDefaultValue[3] = { 0, 0, 64 };
	writeRegister(CS5490_GAIN_FOR_CURRENT, gainDefaultValue);
	writeRegister(CS5490_GAIN_FOR_VOLTAGE, gainDefaultValue);
	writeInstruction(CS5490_CALIBRATION_DC_OFFSET_I_U);
	return true;
}
float* CS5490::GetMeasurements() {
	return measurements;
}
void CS5490::Reset() {
	HAL_GPIO_WritePin(RESET_CS5490_GPIO_Port, RESET_CS5490_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(RESET_CS5490_GPIO_Port, RESET_CS5490_Pin, GPIO_PIN_SET);
	HAL_Delay(80);
}


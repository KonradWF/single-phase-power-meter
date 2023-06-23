
#ifndef __CS5490H__
#define __CS5490H__

#include "main.h"
#include "usart.h"

#define IRMS_INDEX 0
#define URMS_INDEX 1
#define PF_INDEX 2
#define PAVG_INDEX 3
#define QAVG_INDEX 4
#define S_INDEX 5

#define CS5490_RST_PORT GPIOB
#define CS5490_RST_PIN  GPIO_PIN_8

typedef enum CS5490_CommandsMasks
{
	CS5490_READ_REGISTER_MASK = 0,
	CS5490_WRITE_REGISTER_MASK = 64,
	CS5490_PAGE_SELECT_MASK = 128,
	CS5490_INSTRUCTION_MASK = 192,
}cs5490_command_mask_t;

typedef enum CS5490_InstructionSet
{
	CS5490_SW_RESET = 1,
	CS5490_STANDBY = 2,
	CS5490_WAKE_UP = 3,
	CS5490_SINGLE_CONV = 20,
	CS5490_CONTINUOUS_CONV = 21,
	CS5490_HALT_CONV = 24,
	CS5490_CALIBRATION_DC_OFFSET_I_U = 38,
	CS5490_CALIBRATION_AC_OFFSET_I = 49,
	CS5490_CALIBRATION_GAIN_I_U = 52,
}cs5490_instruction_t;

typedef enum CS5490_Registers
{
	CS5490_UART_CONTROL_REGISTER = 7,
	CS5490_STATUS_0_REGISTER = 23,
	CS5490_STATUS_1_REGISTER = 24,
	CS5490_STATUS_2_REGISTER = 25,
	CS5490_CONFIG_2_REGISTER = (16 << 8),
	CS5490_CURRENT_RMS_REGISTER = (16 << 8) + 6,   // 16-bit value, 8-bit page then 8-bit addr 
	CS5490_VOLTAGE_RMS_REGISTER = (16 << 8) + 7,  
	CS5490_ACTIVE_POWER_REGISTER = (16 << 8) + 5,
	CS5490_REACTIVE_POWER_REGISTER = (16 << 8) + 14,
	CS5490_APPARENT_POWER_REGISTER = (16 << 8) + 20,
	CS5490_POWER_FACTOR_REGISTER = (16 << 8) + 21,
	CS5490_GAIN_FOR_CURRENT = (16 << 8) + 33,
	CS5490_GAIN_FOR_VOLTAGE = (16 << 8) + 35,
	CS5490_AC_OFFSET_FOR_CURRENT = (16 << 8) + 37,
	CS5490_DC_OFFSET_FOR_CURRENT = (16 << 8) + 32,
	CS5490_DC_OFFSET_FOR_VOLTAGE = (16 << 8) + 34,
	CS5490_SETTLING_TIME_FOR_CONV = (16 << 8) + 57,
}cs5490_register_t;

class CS5490 {
public:
	CS5490(UART_HandleTypeDef *huart);
	bool CalibrateGain();
	bool CalibrateOffset();
	float* GetMeasurements();
	bool ReadMeasurements();
	void Reset();
	bool UART_SetBaudrate();
	bool SetHPFilter();
private:
	UART_HandleTypeDef *uart;
	float measurements[6];
	bool sendBytes(uint8_t *data, uint32_t length); // Layer for sending data (polling), receiving will use interrupts
	bool readBytes(uint8_t *data, uint32_t length);
	bool readRegister(cs5490_register_t address, uint8_t *destination);
	bool selectPage(cs5490_register_t address);
	bool writeRegister(cs5490_register_t address, uint8_t *data);
	bool writeInstruction(cs5490_instruction_t instruction);

};


#endif

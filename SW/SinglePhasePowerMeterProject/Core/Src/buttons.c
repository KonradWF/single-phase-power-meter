
#include "buttons.h"
#include "gpio.h"
#include "main.h"
#include "state_displays.h"
#include "menu.h"

#define SAMPLES_NUMBER 20
#define BUTTONS_NUMBER 3

#define BUTTON_UP_GPIO_PORT GPIOA
#define BUTTON_UP_GPIO_PIN GPIO_PIN_6
#define BUTTON_DOWN_GPIO_PORT GPIOC
#define BUTTON_DOWN_GPIO_PIN GPIO_PIN_13
#define BUTTON_ENTER_GPIO_PORT GPIOB
#define BUTTON_ENTER_GPIO_PIN GPIO_PIN_9

button_t ButtonsList[BUTTONS_NUMBER];
static uint8_t StateCalibrationButtonPressedFlag = 0;

void ButtonsInit() {
	ButtonsList[BUTTON_UP].gpioPort = BUTTON_UP_GPIO_PORT;
	ButtonsList[BUTTON_DOWN].gpioPort = BUTTON_DOWN_GPIO_PORT;
	ButtonsList[BUTTON_ENTER].gpioPort = BUTTON_ENTER_GPIO_PORT;
	ButtonsList[BUTTON_UP].gpioPin = BUTTON_UP_GPIO_PIN;
	ButtonsList[BUTTON_DOWN].gpioPin = BUTTON_DOWN_GPIO_PIN;
	ButtonsList[BUTTON_ENTER].gpioPin = BUTTON_ENTER_GPIO_PIN;
	ButtonsList[BUTTON_UP].sampleCounter = 0;
	ButtonsList[BUTTON_DOWN].sampleCounter = 0;
	ButtonsList[BUTTON_ENTER].sampleCounter = 0;
	ButtonsList[BUTTON_UP].buttonCallback = MenuNextNode;
	ButtonsList[BUTTON_DOWN].buttonCallback = MenuPreviousNode;
	ButtonsList[BUTTON_ENTER].buttonCallback = GetCurrentNode()->enterCallback;
}

void CheckButtons() {
	for (int i = 0; i < BUTTONS_NUMBER; i++) {
		if (HAL_GPIO_ReadPin(ButtonsList[i].gpioPort, ButtonsList[i].gpioPin)
				== GPIO_PIN_RESET) {
			ButtonsList[i].sampleCounter++;	// If pressed increment counter
		} else if (ButtonsList[i].sampleCounter >= SAMPLES_NUMBER) // Wait for button release
		{
			if (ButtonsList[i].buttonCallback != NULL) {
				ButtonsList[i].buttonCallback(); // If enough samples then execute action
			}
			ButtonsList[i].sampleCounter = 0;          // Set counter to default
		} else {
			ButtonsList[i].sampleCounter = 0; // Not enough samples, button probably pressed for too short period of time
		}
	}
}
void UpdateEnterButton() {
	ButtonsList[BUTTON_ENTER].buttonCallback = GetCurrentNode()->enterCallback;
}
void CalibrationButtonPressed() {
	StateCalibrationButtonPressedFlag = 1;
}
uint8_t GetCalibrationButtonPressedFlag() {
	return StateCalibrationButtonPressedFlag;
}
void ClearCalibrationButtonPressedFlag() {
	StateCalibrationButtonPressedFlag = 0;
}
void MenuSetButtons() {
	ButtonsList[BUTTON_UP].buttonCallback = MenuNextNode;
	ButtonsList[BUTTON_DOWN].buttonCallback = MenuPreviousNode;
	ButtonsList[BUTTON_ENTER].buttonCallback = GetCurrentNode()->enterCallback;
}
void CalibrationSetButtons() {
	ButtonsList[BUTTON_UP].buttonCallback = CalibrationButtonPressed;
	ButtonsList[BUTTON_DOWN].buttonCallback = CalibrationButtonPressed;
	ButtonsList[BUTTON_ENTER].buttonCallback = CalibrationButtonPressed;
}


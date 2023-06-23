
#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

typedef enum ButtonsIndex
{
	BUTTON_UP = 0,
	BUTTON_DOWN  = 1,
	BUTTON_ENTER = 2,
}button_index_t;

typedef struct Button
{
	GPIO_TypeDef* gpioPort;
	uint16_t gpioPin;
	uint16_t sampleCounter;
	void(*buttonCallback)(void);
}button_t;

void ButtonsInit();
void CheckButtons();
void UpdateEnterButton();
void MenuSetButtons();
void CalibrationSetButtons();
void ClearCalibrationButtonPressedFlag();
uint8_t GetCalibrationButtonPressedFlag();
#ifdef __cplusplus
}
#endif
#endif /* INC_BUTTONS_H_ */

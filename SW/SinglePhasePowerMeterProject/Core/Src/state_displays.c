
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


#include "state_displays.h"
#include "st7789.h"
#include <stdio.h>

#define THEME_COLOUR_1 0x9882

void stateMenuDefaultDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_DrawFilledRectangle(0, 0, 240, 30, THEME_COLOUR_1);
	ST7789_WriteString(100, 5, "MENU", Font_11x18, WHITE, THEME_COLOUR_1);
	ST7789_WriteString(37, 50, "CALIBRATION", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(30, 120, "MEASUREMENTS", Font_16x26, WHITE, GRAY);
}
void menuHighlightIndex(uint32_t index) {
	if (index == 0) {
		ST7789_DrawRectangle(0, 30, 239, 90, RED);
		ST7789_DrawRectangle(0, 100, 239, 160, GRAY);
		ST7789_DrawRectangle(0, 170, 239, 230, GRAY);
	} else if (index == 1) {
		ST7789_DrawRectangle(0, 30, 239, 90, GRAY);
		ST7789_DrawRectangle(0, 100, 239, 160, RED);
		ST7789_DrawRectangle(0, 170, 239, 230, GRAY);

	} else if (index == 2) {
		ST7789_DrawRectangle(0, 30, 239, 90, GRAY);
		ST7789_DrawRectangle(0, 100, 239, 160, GRAY);
		ST7789_DrawRectangle(0, 170, 239, 230, RED);
	}
}

void stateCalibrationDefaultDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_DrawFilledRectangle(0, 0, 240, 30, THEME_COLOUR_1);
	ST7789_WriteString(60, 5, "CALIBRATION", Font_11x18, WHITE, THEME_COLOUR_1);
	ST7789_WriteString(74, 50, "OFFSET", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(90, 120, "GAIN", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(90, 190, "EXIT", Font_16x26, WHITE, GRAY);
}

void stateMeasurementsP1DefaultDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_WriteString(102, 5, "1/2", Font_11x18, WHITE, GRAY);
	ST7789_DrawFilledRectangle(0, 30, 240, 60, THEME_COLOUR_1);
	ST7789_DrawFilledRectangle(0, 100, 240, 60, THEME_COLOUR_1);
	ST7789_DrawFilledRectangle(0, 170, 240, 60, THEME_COLOUR_1);

	ST7789_WriteString(35, 50, "Irms = ", Font_11x18, WHITE, THEME_COLOUR_1);

	ST7789_WriteString(35, 120, "Urms = ", Font_11x18, WHITE, THEME_COLOUR_1);

	ST7789_WriteString(35, 190, " PF  = ", Font_11x18, WHITE, THEME_COLOUR_1);

}

void stateMeasurementsP2DefaultDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_WriteString(102, 5, "2/2", Font_11x18, WHITE, GRAY);
	ST7789_DrawFilledRectangle(0, 30, 240, 60, THEME_COLOUR_1);
	ST7789_DrawFilledRectangle(0, 100, 240, 60, THEME_COLOUR_1);
	ST7789_DrawFilledRectangle(0, 170, 240, 60, THEME_COLOUR_1);

	ST7789_WriteString(35, 50, "Pavg = ", Font_11x18, WHITE, THEME_COLOUR_1);
	ST7789_WriteString(35, 120, "Qavg = ", Font_11x18, WHITE, THEME_COLOUR_1);
	ST7789_WriteString(35, 190, "  S  = ", Font_11x18, WHITE, THEME_COLOUR_1);
}

void stateMeasurementsP1UpdateDisplay(float u, float i, float pf) {
	char str[20];
	sprintf(str, "Irms = %05.2f A", min(i, 99.99));
	ST7789_WriteString(35, 50, str, Font_11x18, WHITE, THEME_COLOUR_1);

	sprintf(str, "Urms = %06.2f V", min(u, 999.99));
	ST7789_WriteString(35, 120, str, Font_11x18, WHITE, THEME_COLOUR_1);
	sprintf(str, " PF  = %06.2f %%", min(pf, 100.00));
	ST7789_WriteString(35, 190, str, Font_11x18, WHITE, THEME_COLOUR_1);

}
void stateMeasurementsP2UpdateDisplay(float p, float q, float s) {
	char str[20];
	sprintf(str, "Pavg = %06.1f W", min(p, 9999.9));
	ST7789_WriteString(35, 50, str, Font_11x18, WHITE, THEME_COLOUR_1);

	sprintf(str, "Qavg = %06.1f W", min(q, 9999.9));
	ST7789_WriteString(35, 120, str, Font_11x18, WHITE, THEME_COLOUR_1);
	sprintf(str, "  S  = %06.1f W", min(s, 9999.9));
	ST7789_WriteString(35, 190, str, Font_11x18, WHITE, THEME_COLOUR_1);
}
void stateCalibrationGainIDLEDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_DrawFilledRectangle(0, 0, 240, 30, THEME_COLOUR_1);
	ST7789_WriteString(35, 5, "GAIN CALIBRATION", Font_11x18, WHITE,
			THEME_COLOUR_1);

	ST7789_WriteString(80, 70, "APPLY", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(45, 110, "REFERENCE", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(75, 150, "SIGNAL", Font_16x26, WHITE, GRAY);
}
void stateCalibrationOffsetIDLEDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_DrawFilledRectangle(0, 0, 240, 30, THEME_COLOUR_1);
	ST7789_WriteString(22, 5, "OFFSET CALIBRATION", Font_11x18, WHITE,
			THEME_COLOUR_1);

	ST7789_WriteString(88, 70, "OPEN", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(94, 110, "THE", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(65, 150, "CIRCUIT", Font_16x26, WHITE, GRAY);
}
void stateCalibrationFinalDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_WriteString(30, 50, "CALIBRATION", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(45, 120, "COMPLETED", Font_16x26, WHITE, GRAY);
}
void stateCalibrationPerformingCalibrationDisplay() {
	ST7789_DrawFilledRectangle(0, 0, 240, 240, GRAY);
	ST7789_WriteString(40, 50, "PERFORMING", Font_16x26, WHITE, GRAY);
	ST7789_WriteString(30, 120, "CALIBRATION", Font_16x26, WHITE, GRAY);
}



#include <cs5490_c_wrapper.h>
#include "CS5490.h"
#include "state_displays.h"
#include "buttons.h"
#include "menu.h"

extern CS5490* powerMeter;

void CS5490_GainCalibrationProcedure()
{
	stateCalibrationGainIDLEDisplay();
	CalibrationSetButtons();
	while(!GetCalibrationButtonPressedFlag())
	{
		CheckButtons();
		HAL_Delay(25);
	}
	stateCalibrationPerformingCalibrationDisplay();
	HAL_Delay(2000);
	//powerMeter->CalibrateGain();

	MenuSetButtons();
	ClearCalibrationButtonPressedFlag();
	stateCalibrationFinalDisplay();
	HAL_Delay(2000);
	LeaveSubMenu();
}
void CS5490_OffsetCalibrationProcedure()
{
	stateCalibrationOffsetIDLEDisplay();
	CalibrationSetButtons();
	while(!GetCalibrationButtonPressedFlag())
	{
		CheckButtons();
		HAL_Delay(25);
	}
	stateCalibrationPerformingCalibrationDisplay();
	HAL_Delay(2000);
	//powerMeter->CalibrateOffset();
	MenuSetButtons();
	ClearCalibrationButtonPressedFlag();
	stateCalibrationFinalDisplay();
	HAL_Delay(2000);
	LeaveSubMenu();
}



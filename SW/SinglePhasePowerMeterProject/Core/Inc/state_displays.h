
#ifndef INC_STATE_DISPLAYS_H_
#define INC_STATE_DISPLAYS_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

void stateMenuDefaultDisplay();
void menuHighlightIndex(uint32_t index);

void stateCalibrationDefaultDisplay();
void stateCalibrationGainIDLEDisplay();
void stateCalibrationOffsetIDLEDisplay();
void stateCalibrationPerformingCalibrationDisplay();
void stateCalibrationFinalDisplay();

void stateMeasurementsP1DefaultDisplay();
void stateMeasurementsP1UpdateDisplay(float u, float i, float pf);
void stateMeasurementsP2DefaultDisplay();
void stateMeasurementsP2UpdateDisplay(float p, float q, float s);

#ifdef __cplusplus
}
#endif
#endif /* INC_STATE_DISPLAYS_H_ */

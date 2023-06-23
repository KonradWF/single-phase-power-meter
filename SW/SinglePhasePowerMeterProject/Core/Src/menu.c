
#include <cs5490_c_wrapper.h>
#include "menu.h"
#include"state_displays.h"
#include "buttons.h"

static menu_node_t *currentNode = NULL;

menu_node_t sub_cal_exit, sub_meas_one, sub_meas_two;
menu_node_t sub_cal_gain, sub_cal_offset, measurements;


menu_node_t calibration = {0,"CALIBRATION", NULL, &measurements, &sub_cal_offset, NULL,
		EnterSubMenu, menuHighlightIndex, NULL, stateMenuDefaultDisplay};
menu_node_t measurements = {1,"MEASUREMENTS", &calibration, NULL, &sub_meas_one, NULL,
		EnterSubMenu, menuHighlightIndex, NULL, stateMenuDefaultDisplay};
menu_node_t sub_cal_offset = {0,"CAL_OFFSET", NULL, &sub_cal_gain, NULL, &calibration,
		CS5490_OffsetCalibrationProcedure, menuHighlightIndex, NULL, stateCalibrationDefaultDisplay};
menu_node_t sub_cal_gain = {1,"CAL_GAIN", &sub_cal_offset, &sub_cal_exit, NULL, &calibration,
		CS5490_GainCalibrationProcedure, menuHighlightIndex, NULL, stateCalibrationDefaultDisplay};
menu_node_t sub_cal_exit = {2,"CAL_EXIT", &sub_cal_gain, NULL, NULL, &calibration,
		LeaveSubMenu, menuHighlightIndex, NULL, stateCalibrationDefaultDisplay};
menu_node_t sub_meas_one = {0,"MEAS_1", NULL, &sub_meas_two, NULL, &measurements,
		LeaveSubMenu, NULL, stateMeasurementsP1DefaultDisplay, stateMeasurementsP1DefaultDisplay};
menu_node_t sub_meas_two = {1,"MEAS_2", &sub_meas_one, NULL, NULL, &measurements,
		LeaveSubMenu, NULL, stateMeasurementsP2DefaultDisplay, stateMeasurementsP2DefaultDisplay};

void MenuNextNode() {
	if (currentNode->next != NULL) {
		currentNode = currentNode->next;
	} else {
		menu_node_t *temp = currentNode;
		while (temp->prev != NULL) {
			temp = temp->prev;
		}
		currentNode = temp;
	}
	if (currentNode->highlightCallback != NULL) {
		currentNode->highlightCallback(currentNode->index);
	} else if (currentNode->changeDisplayCallback != NULL) {
		currentNode->changeDisplayCallback();
	}
	UpdateEnterButton();
}
void MenuPreviousNode() {
	if (currentNode->prev != NULL) {
		currentNode = currentNode->prev;
	} else {
		menu_node_t *temp = currentNode;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		currentNode = temp;
	}
	if (currentNode->highlightCallback != NULL) {
		currentNode->highlightCallback(currentNode->index);
	} else if (currentNode->changeDisplayCallback != NULL) {
		currentNode->changeDisplayCallback();
	}
	UpdateEnterButton();
}

void EnterSubMenu() {
	if (currentNode->child != NULL) {
		currentNode = currentNode->child;
		currentNode->defaultDisplayCallback();
		if (currentNode->highlightCallback != NULL) {
			currentNode->highlightCallback(currentNode->index);
		}
		UpdateEnterButton();
	}
}

void LeaveSubMenu() {
	if (currentNode->parent != NULL) {
		currentNode = currentNode->parent;
		currentNode->defaultDisplayCallback();
		if (currentNode->highlightCallback != NULL) {
			currentNode->highlightCallback(currentNode->index);
		}
		UpdateEnterButton();
	}
}

void MenuInit() {
	stateMenuDefaultDisplay();
	currentNode = &measurements;
	MenuNextNode();
}
const menu_node_t* GetCurrentNode() {
	return currentNode;
}

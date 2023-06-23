
#ifndef INC_MENU_H_
#define INC_MENU_H_


#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct MenuNode menu_node_t;

typedef struct MenuNode
{
	uint32_t index;
	char* name;
	menu_node_t* prev;
	menu_node_t* next;
	menu_node_t* child;
	menu_node_t* parent;
	void (*enterCallback)(void);
	void (*highlightCallback)(uint32_t index);
	void (*changeDisplayCallback)();
	void (*defaultDisplayCallback)();
}menu_node_t;


void MenuNextNode();
void MenuPreviousNode();
void EnterSubMenu();
void LeaveSubMenu();
uint16_t GetNodeLevel();
void MenuInit();
const menu_node_t* GetCurrentNode();
#ifdef __cplusplus
}
#endif

#endif /* INC_MENU_H_ */

#ifdef GEKKO
#include "inputDef.h"
#include "wii.h"
#include "wupc.h"
#include <wupc/wupc.h>
//The percentage of the total magnitude that the stick must be moved to register as pressed
#define PRO_SENSITIVITY 0.75
//Gets the classic input
#define PRO_MAX 1024
int wupcInput(int wpadnum)
{
	int leveldir = 0;
	int xdir = 0;
	int ydir = 0;
	int joyX, joyY;
	struct WUPCData *proController = WUPC_Data(wpadnum);
	if (proController == NULL)
	{
		return INPUT_NONE;
	}
	joyX = WUPC_lStickX(wpadnum);
	joyY = WUPC_lStickY(wpadnum);
	if (joyY <= PRO_SENSITIVITY*PRO_MAX*-1)
		ydir +=1;
	if (joyX >= PRO_SENSITIVITY*PRO_MAX)
		xdir += 1;
	if  (joyY >= PRO_SENSITIVITY*PRO_MAX)
		ydir -= 1;
	if (joyX <= PRO_SENSITIVITY*PRO_MAX*-1)
		xdir -= 1;
	if (proController->button & WPAD_CLASSIC_BUTTON_HOME)
		return BUTTON_MENU;
	if (proController->button & WPAD_CLASSIC_BUTTON_FULL_R||proController->button & WPAD_CLASSIC_BUTTON_PLUS)
		leveldir += 1;
	if (proController->button & WPAD_CLASSIC_BUTTON_FULL_L||proController->button & WPAD_CLASSIC_BUTTON_MINUS)
		leveldir-= 1;
	if (leveldir <= -1)
		return BUTTON_LEVEL_PREV;
	if (leveldir >= 1)
		return BUTTON_LEVEL_NEXT;
	if (proController->button & WPAD_CLASSIC_BUTTON_B||proController->button & WPAD_CLASSIC_BUTTON_Y)
		return BUTTON_1;
	if (proController->button & WPAD_CLASSIC_BUTTON_A||proController->button & WPAD_CLASSIC_BUTTON_X)
		return BUTTON_2;
	if (proController->button & WPAD_CLASSIC_BUTTON_LEFT)
		xdir-=1;
	if (proController->button & WPAD_CLASSIC_BUTTON_RIGHT)
		xdir += 1;
	if (xdir <= -1)
		return B_LEFT;
	if (xdir >= 1)
		return B_RIGHT;
	if (proController->button & WPAD_CLASSIC_BUTTON_UP)
		ydir-=1;
	if (proController->button & WPAD_CLASSIC_BUTTON_DOWN)
		ydir += 1;
	if (ydir <= -1)
		return B_UP;
	if (ydir >= 1)
		return B_DOWN;
	if (proController->button & WPAD_CLASSIC_BUTTON_FULL_L||proController->button & WPAD_CLASSIC_BUTTON_ZR)
		return B_ANY;
	if (proController->button & WPAD_CLASSIC_BUTTON_ZL || proController->button & WPAD_CLASSIC_BUTTON_ZR)
		return BUTTON_RESET;
	return INPUT_NONE;
}
#endif
#ifdef GEKKO
#include "inputDef.h"
#include "wii.h"
//The percentage of the total magnitude that the stick must be moved to register as pressed
#define CLASSIC_SENSITIVITY 0.75
//Gets the classic input
int classicInput(int wpadnum)
{
	struct expansion_t exptype;
	int leveldir = 0;
	int xdir = 0;
	int ydir = 0;
	WPADData *classic = WPAD_Data(wpadnum);
	switch(wpadnum)
	{
	case 0:
		WPAD_Expansion(WPAD_CHAN_0, &exptype);
		break;
	case 1:
		WPAD_Expansion(WPAD_CHAN_1, &exptype);
		break;
	case 2:
		WPAD_Expansion(WPAD_CHAN_2, &exptype);
		break;
	case 3:
		WPAD_Expansion(WPAD_CHAN_3, &exptype);
		break;
	default:
		break;
	}
	if ((exptype.classic.ljs.ang >=315) || (exptype.classic.ljs.ang<=45) && (exptype.classic.ljs.mag >= CLASSIC_SENSITIVITY))
		ydir -=1;
	if ((exptype.classic.ljs.ang >=45) && (exptype.classic.ljs.ang<=135) && (exptype.classic.ljs.mag >= CLASSIC_SENSITIVITY))
		xdir += 1;
	if ((exptype.classic.ljs.ang >=135) && (exptype.classic.ljs.ang<=225) && (exptype.classic.ljs.mag >= CLASSIC_SENSITIVITY))
		ydir += 1;
	if ((exptype.classic.ljs.ang >=225) && (exptype.classic.ljs.ang<=315) && (exptype.classic.ljs.mag >= CLASSIC_SENSITIVITY))
		xdir -= 1;
	if ((exptype.classic.rjs.ang >=315) || (exptype.classic.rjs.ang<=45) && (exptype.classic.rjs.mag >= CLASSIC_SENSITIVITY))
		ydir -=1;
	if ((exptype.classic.rjs.ang >=45) && (exptype.classic.rjs.ang<=135) && (exptype.classic.rjs.mag >= CLASSIC_SENSITIVITY))
		xdir += 1;
	if ((exptype.classic.rjs.ang >=135) && (exptype.classic.rjs.ang<=225) && (exptype.classic.rjs.mag >= CLASSIC_SENSITIVITY))
		ydir += 1;
	if ((exptype.classic.rjs.ang >=225) && (exptype.classic.rjs.ang<=315) && (exptype.classic.rjs.mag >= CLASSIC_SENSITIVITY))
		xdir -= 1;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_HOME)
		return BUTTON_MENU;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_FULL_R||classic->btns_h & WPAD_CLASSIC_BUTTON_PLUS)
		leveldir += 1;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_FULL_L||classic->btns_h & WPAD_CLASSIC_BUTTON_MINUS)
		leveldir-= 1;
	if (leveldir <= -1)
		return BUTTON_LEVEL_PREV;
	if (leveldir >= 1)
		return BUTTON_LEVEL_NEXT;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_B||classic->btns_h & WPAD_CLASSIC_BUTTON_Y)
		return BUTTON_1;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_A||classic->btns_h & WPAD_CLASSIC_BUTTON_X)
		return BUTTON_2;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_LEFT)
		xdir-=1;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_RIGHT)
		xdir += 1;
	if (xdir <= -1)
		return B_LEFT;
	if (xdir >= 1)
		return B_RIGHT;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_UP)
		ydir-=1;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_DOWN)
		ydir += 1;
	if (ydir <= -1)
		return B_UP;
	if (ydir >= 1)
		return B_DOWN;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_ZL ||  classic->btns_h & WPAD_CLASSIC_BUTTON_ZR)
		return BUTTON_RESET;
	if (classic->btns_h & WPAD_CLASSIC_BUTTON_FULL_L||classic->btns_h & WPAD_CLASSIC_BUTTON_ZR)
		return B_ANY;
	return INPUT_NONE;
}
#endif
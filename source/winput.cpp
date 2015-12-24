#ifdef GEKKO
#include "wii.h"
#include "inputDef.h"
#include "defs.h"
bool nunchuk_dir(double, int, int);
//Which expansion is plugged in
struct expansion_t data;
//Get the Wiiremote input
//Todo: Move this to Wiiuse for portability???
int WInput(u16 WButtonsDown,bool checkNunchuk,int wpadnum)
{
	double mag = 0.0;
	double ang = 0.0;
	if (checkNunchuk == 1)
	{
		WPAD_Expansion(wpadnum, &data);
		mag = data.nunchuk.js.mag;
		ang = data.nunchuk.js.ang;
	}
	else
	{
		mag = 0;
		ang = 0;
	}
	int leveldir = 0;
	int xdir = 0;
	int ydir = 0;
	if (WButtonsDown & WPAD_BUTTON_HOME)
		return BUTTON_MENU;
	if (WButtonsDown & WPAD_BUTTON_PLUS)
		leveldir += 1;
	if (WButtonsDown & WPAD_BUTTON_MINUS)
		leveldir-= 1;
	if (leveldir <= -1)
		return BUTTON_LEVEL_PREV;
	if (leveldir >= 1)
		return BUTTON_LEVEL_NEXT;
	if (WButtonsDown & WPAD_BUTTON_1)
		return BUTTON_1;
	if (WButtonsDown & WPAD_BUTTON_B)
		return BUTTON_RESET;
	if (checkNunchuk == 1)
	{ 
		if (WButtonsDown & WPAD_NUNCHUK_BUTTON_Z)
			return BUTTON_1;
		if (WButtonsDown & WPAD_NUNCHUK_BUTTON_C)
			return BUTTON_2;
	}
	if (WButtonsDown & WPAD_BUTTON_2)
		return BUTTON_2;
	if (checkNunchuk == 1)
	{
		//Nunchuck Up
		if (nunchuk_dir(mag, ang, D_UP))
			ydir -= 1;
		//Nunchuck Right
		else if (nunchuk_dir(mag, ang, D_RIGHT))
			xdir += 1;
		//Nunchuck Down
		else if (nunchuk_dir(mag, ang, D_DOWN))
			ydir += 1;
		//Nunchuck Left
		else if (nunchuk_dir(mag, ang, D_LEFT))
			xdir -= 1;
		if (WButtonsDown & WPAD_BUTTON_UP)
			ydir-=1;
		if (WButtonsDown & WPAD_BUTTON_DOWN)
			ydir += 1;
		if (WButtonsDown & WPAD_BUTTON_RIGHT)
			xdir +=1;
		if (WButtonsDown & WPAD_BUTTON_LEFT)
			xdir -= 1;
	}
	else
	{
		if (WButtonsDown & WPAD_BUTTON_UP)
			xdir -= 1;
		if (WButtonsDown & WPAD_BUTTON_DOWN)
			xdir += 1;
		if (WButtonsDown & WPAD_BUTTON_RIGHT)
			ydir -= 1;
		if (WButtonsDown & WPAD_BUTTON_LEFT)
			ydir += 1;
	}
	if (xdir <= -1)
		return B_LEFT;
	if (xdir >= 1)
		return B_RIGHT;
	if (ydir <= -1)
		return B_UP;
	if (ydir >= 1)
		return B_DOWN;
	if (WButtonsDown & WPAD_BUTTON_A)
		return B_ANY;
	return INPUT_NONE;
}
//Check if the nunchuck is pushed in the given direction
bool nunchuk_dir(double mag, int angle, int dir)
{
	if (((angle >= 315 || angle < 45) && mag >= 0.75 && dir == D_UP)
		|| (angle >= 45 && angle < 135 && mag >= 0.75 && dir == D_RIGHT)
		|| (angle >= 135 && angle< 225 && mag >= 0.75 && dir == D_DOWN)
		|| (angle >= 225 && angle< 315 && mag >= 0.75 && dir == D_LEFT))
		return 1;
	else return D_NONE;
}
#endif
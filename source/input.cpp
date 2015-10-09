#include "inputDef.h"
#include "input.h"

bool useMouse = 0;
extern int mouseX;
extern int mouseY;
extern int numJoysticks;
int secondaryControl;
int controltype = CONTROLLER_NONE;
extern int chosenController;
extern int hatDirection;
extern int mouseInput;
struct mousePosition
{
	int x;
	int y;
};
extern bool replayEnabled;
int getNextReplayMove();

#ifdef GEKKO
#include "wii.h"
#include <gccore.h>

extern bool showCursor;
u32 exp_type;
int WInput(u16,bool,int);
int classicInput(int);
int wupcInput(int wpadnum);
u16 WButtonsDown[4];
int wpadnum = 0;
bool mouseConnected = 0;
ir_t ir;
int gcsensitivity = 80;
int padnum = 0;
u16 GCButtonsDown[4];
int GCInput(u16 GCButtonsDown);
int GCControlPort;

extern char HWButton;
int wupcInput(int);
//Wii
//Chose which controller based on initial input
//Possible choices:
//Wiimote on side
//Wiimote with nunchuck
//Wiimote with classic controller
//USB keyboard
//Gamecube controller
//Wii U Pro Controller
//Uses the first input channel that recieves data as well
int pickControls()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (wupcInput(i) != INPUT_NONE)
		{
			wpadnum = i;
			return CONTROLLER_WUPC;
		}
		WPAD_Probe(i, &exp_type);
		if (WInput(WButtonsDown[i],1,i) != INPUT_NONE)
		{
			wpadnum = i;
			if (exp_type == WPAD_EXP_NONE)
				return CONTROLLER_WIIMOTE;
			if (exp_type == WPAD_EXP_NUNCHUK)
				return CONTROLLER_NUNCHUK;
		}
		if (exp_type == WPAD_EXP_CLASSIC)
		{
			if (classicInput(i) != INPUT_NONE)
			{
				wpadnum = i;
				return CONTROLLER_CLASSIC;
			}
		}
	}
	if (checkGC() == true)
	{
		return CONTROLLER_GAMECUBE;
	}
	if (keyInput() != INPUT_NONE)
	{
		return CONTROLLER_KEYBOARD;
	}
	return CONTROLLER_NONE;
}
#endif
#ifndef GEKKO
//Checks for joystick input or keyboard input
//Whichever recieves a button press first is used
int pickControls()
{
	if (joyInput() != INPUT_NONE)
		return CONTROLLER_JOYSTICK;
	if (keyInput() != INPUT_NONE)
		return CONTROLLER_KEYBOARD;
	return CONTROLLER_NONE;
}
#endif
#ifdef GEKKO
//Wii
//Check all of the pads input states
void pad_set()
{
	for (int i = 0; i < 4; i++)
		GCButtonsDown[i] = PAD_ButtonsHeld(i);
}
#endif
//Return the chosen control type
int getControlType()
{
	return controltype;
}
#ifdef GEKKO
//Initializes the input
void inputInit()
{
	//If a mouse is connected, use it
	if (MOUSE_IsConnected())
	{
		mouseConnected = true;
		showCursor = true;
		MOUSE_Init();
		secondaryControl = CONTROLLER_MOUSE;
	}
	else
	{
		secondaryControl = CONTROLLER_POINTER;
	}
	mouseX = 0;
	mouseY = 0;
	//Initialize the Wiimote data
	WPAD_SetDataFormat(0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(0, 640, 480);
	//Initialize the various control schemes
	KEYBOARD_Init(NULL);
	WUPC_Init();
	WPAD_Init();
	PAD_Init();
}
//Updates the pointer based on what is used to input it
void updatePointer()
{
	WPAD_ScanPads();
	mouseConnected = MOUSE_IsConnected();
	if (secondaryControl == CONTROLLER_POINTER)
	{
		WPAD_IR(wpadnum, &ir);
		mouseX = ir.x;
		mouseY = ir.y;
		if (ir.smooth_valid == false) {
			mouseX = -1;
			mouseY = -1;
		}
	}
}
//Decide which input to use and return the function
//Input is chosen based on which input device is used first
//Any controller number may be used
int determineInput(bool mouse)
{
	PAD_ScanPads();
	WPAD_ScanPads();
	for (int i = 0; i < 4; i++)
		WButtonsDown[i] = WPAD_ButtonsHeld(i);
	updatePointer();
	pad_set();
	if (mouse)
	{
		if (mouseInput != INPUT_NONE)
			return mouseInput;
	}
	switch (controltype)
	{
	case CONTROLLER_NONE:
		controltype = pickControls();
		return INPUT_NONE;
		break;
	case CONTROLLER_CLASSIC:
		return classicInput(wpadnum);
		break;
	case CONTROLLER_WIIMOTE:
		return WInput(WButtonsDown[wpadnum],0,wpadnum);
		break;
	case CONTROLLER_NUNCHUK:
		return WInput(WButtonsDown[wpadnum],1,wpadnum);
		break;
	case CONTROLLER_GAMECUBE:
		return GCInput(GCButtonsDown[padnum-1]);
		break;
	case CONTROLLER_KEYBOARD:
		return keyInput();
		break;
	case CONTROLLER_WUPC:
		return wupcInput(wpadnum);
	default:
		break;
	}	
	return INPUT_NONE;
}
#endif
#ifndef GEKKO
//Input is chosen based on which device is used first
int determineInput(bool mouse)
{
	if (mouse)
	{
		if (mouseInput != INPUT_NONE)
			return mouseInput;
	}
	if (controltype == CONTROLLER_KEYBOARD)
		return keyInput();
	else if (controltype == CONTROLLER_JOYSTICK)
		return joyInput();
	else
		controltype = pickControls();
	return INPUT_NONE;
}
#else
//Check for gamecube input
bool checkGC()
{
	for (int i = 0; i < 4; i++)
		if (GCInput(GCButtonsDown[i]) != INPUT_NONE)
		{
			padnum = i+1;
			return true;
		}
		return false;
}
//Return which control port is being used for the gamecube controller
int getControlPort()
{
	return GCControlPort;
}
#endif
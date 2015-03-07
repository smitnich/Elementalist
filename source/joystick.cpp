#ifndef GEKKO
#ifdef _WIN32
#include <SDL/SDL.h>
#elif LINUX
#include <SDL/SDL.h>
#endif
#include "base.h"
#include "input_def.h"
#define JOYSTICK_MAX 32767;
#define MAX_BUTTONS 32
//The direction that the hat is currently facing
int hatDirection = SDL_HAT_CENTERED;
//What value the magnitude of the stick must reach in order to be an event
int sensitivityThreshold = 0;
//The coordinates of the joystick
int joystickX = 0;
int joystickY = 0;
//Which axis to use for movement on the x plane
int moveAxisX = 0;
//Which axis to use for movement on the y plane
int moveAxisY = 1;
//Which axis to use for the Trigger
int triggerAxis1 = 2;
//Which axis to use for the other trigger, if possible
int triggerAxis2 = 5;
//The number of joysticks plugged in
int numJoysticks = -1;
//Which is the first button
int buttonA = 0;
//Which is the second button
int buttonB = 1;
//The next button
int buttonNext = 5;
//The previous button
//Todo: allow these to be rebound
int buttonPrev = 4;
//The exit button
int buttonExit = 7;
//The number of axes on a controlelr
int numAxes = -1;
//The stupid xboxAxis used for shoulder buttons
//Todo: fix for Linux (Two seperate axes)
int xboxAxis = 0;
int xboxAxis2 = 0;
//The amount that the axes must be depressed in order to register an input
//Not counting the amount being substracted by the other axis
double xboxAxisSensitivity = 0.7;
//The actual value to be used
int xboxAxisSensitivityThreshold;
//Which controller will be used
int chosenController = -1;
//Up to 4 joysticks can be used
//Todo: More?
SDL_Joystick *joystick[MAX_JOYSTICK];
//Used to calculate the sensitivty threshold
double joystickSensitivity = 0.8;
//An array of which buttons have been pressed
bool joyButtonsDown[MAX_BUTTONS] = {0};
//Updates hat, joystick, and button values
//controllerCheck makes sure that the event comes from the proper controller
void updateJoystick(SDL_Event event, int controllerCheck)
{
	int x = event.type;
	if (event.type == SDL_JOYAXISMOTION)
	{
		//If a controller hasn't been chosen, use this controller and return
		if (controllerCheck == -1)
		{
			chosenController = event.jaxis.which;
			return;
		}
		//Make sure its the proper controller
		if (event.jaxis.which == controllerCheck)
		{
			//Update the axis values
			if (event.jaxis.axis == moveAxisX)
				joystickX = event.jaxis.value;
			else if (event.jaxis.axis == moveAxisY)
				joystickY = event.jaxis.value;
			else if (event.jaxis.axis == triggerAxis1)
				xboxAxis = event.jaxis.value;
#ifdef LINUX
			else if (event.jaxis.axis == triggerAxis2)
				xboxAxis2 = event.jaxis.value;
#endif
		}
	}
	//Update the Joystick hat (ie dpad)
	//Todo: Figure out why this doesn't work on Ubuntu
	else if (event.type == SDL_JOYHATMOTION)
	{
		if (controllerCheck == -1)
		{
			chosenController = event.jhat.which;
		}
		if (event.jhat.which == controllerCheck)
		{
			if (event.jhat.value & SDL_HAT_LEFT)
				hatDirection = B_LEFT;
			else if (event.jhat.value & SDL_HAT_RIGHT)
				hatDirection = B_RIGHT;
			else if (event.jhat.value & SDL_HAT_UP)
				hatDirection = B_UP;
			else if (event.jhat.value & SDL_HAT_DOWN)
				hatDirection = B_DOWN;
            else if (event.jhat.value == SDL_HAT_CENTERED)
                hatDirection = 0;
		}
	}
	//Update the button array
	else if (event.type == SDL_JOYBUTTONDOWN)
	{
		if (controllerCheck == -1)
		{
			chosenController = event.jbutton.which;
			return;
		}
		if (event.button.which == controllerCheck)
			joyButtonsDown[event.jbutton.button] = 1;
	}
	else if (event.type == SDL_JOYBUTTONUP)
	{
		if (controllerCheck == -1)
		{
			chosenController = event.jbutton.which;
			return;
		}
		if (event.button.which == controllerCheck)
			joyButtonsDown[event.jbutton.button] = 0;
	}
}
//Initialize the sensitivity values and open all joysticks connected (up to 4)
void joystickInit()
{
	sensitivityThreshold = joystickSensitivity*JOYSTICK_MAX;
	xboxAxisSensitivityThreshold = xboxAxisSensitivity*JOYSTICK_MAX;
	numJoysticks = SDL_NumJoysticks();
	int i;
	for (i = 0; i < MAX_JOYSTICK && i < numJoysticks; i++)
	{
		joystick[i] = SDL_JoystickOpen( i );
	}
}
//Use the previously gathered information to move the player
int joyInput()
{
	int xdir = 0;
	int ydir = 0;
	int levelDir = 0;
	int x = joyButtonsDown[buttonExit];
	if (joyButtonsDown[buttonExit])
		return BUTTON_MENU;
	if (joystickX >= sensitivityThreshold)
	{
		xdir += 1;
	}
	else if (joystickX <= sensitivityThreshold*-1)
	{
		xdir -= 1;
	}
	if (joystickY >= sensitivityThreshold)
	{
		ydir += 1;
	}
	else if (joystickY <= sensitivityThreshold*-1)
	{
		ydir -= 1;
	}
	if (joyButtonsDown[buttonA])
		return BUTTON_1;
	if (joyButtonsDown[buttonB])
		return BUTTON_2;
	if (hatDirection == B_DOWN)
			ydir += 1;
	else if (hatDirection == B_UP)
			ydir -= 1;
	else if (hatDirection == B_LEFT)
			xdir -= 1;
	else if (hatDirection == B_RIGHT)
			xdir += 1;
	if (xdir < 0)
		return B_LEFT;
	else if (xdir > 0)
		return B_RIGHT;
	else if (ydir < 0)
		return B_UP;
	else if (ydir > 0)
		return B_DOWN;
    //Linux finds two joystick axes, windows 1
    #ifndef LINUX
	if (xboxAxis >= xboxAxisSensitivityThreshold)
		levelDir -= 1;
	else if (xboxAxis <= xboxAxisSensitivityThreshold*-1)
		levelDir += 1;
    #else
    if (xboxAxis <= xboxAxisSensitivityThreshold*-1)
        levelDir += 1;
    if (xboxAxis2 <= xboxAxisSensitivityThreshold*-1 )
        levelDir -= 1;
    #endif
	if (joyButtonsDown[buttonNext])
		levelDir += 1;
	else if (joyButtonsDown[buttonPrev])
		levelDir -= 1;
	if (levelDir < 0)
		return BUTTON_LEVEL_PREV;
	else if (levelDir > 0)
		return BUTTON_LEVEL_NEXT;
	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		if (joyButtonsDown[i])
			return B_ANY;
	}
	return INPUT_NONE;
}
#endif

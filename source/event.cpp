//Choose the proper SDL path based on the operating system
#ifdef GEKKO
#include <SDL/sdl.h>
#include <SDL/SDL_events.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#endif
void cleanup();
void doMouse(SDL_Event);
void updateKeys(SDL_Event);
void updateJoystick(SDL_Event,int);
extern int hatDirection;
extern int chosenController;
void checkEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
			doMouse(event);
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			updateKeys(event);
			break;
		//Not wii
		#ifndef GEKKO
		//Update the joystick values
		case SDL_JOYAXISMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYHATMOTION:
		case SDL_JOYBALLMOTION:
		case SDL_JOYBUTTONUP:
			updateJoystick(event,chosenController);
			break;
		#endif
		//Exit button pressed
		case SDL_QUIT:
			cleanup();
			break;
		default:
			break;
		}
	}
}

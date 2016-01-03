#ifdef GEKKO
#include "wii.h"
#include <wupc/wupc.h>
#include <ogcsys.h>
#include <unistd.h>
#include <wiikeyboard/keyboard.h>
#include <ogc/usbmouse.h>
#include <SDL/SDL.h>
#elif _WIN32
#include <SDL/SDL.h>
#elif LINUX
#include <SDL/SDL.h>
#endif
void joystickInit();
void inputInit();
int getControlPort();
int determineInput(bool mouse);
int keyInput();
int joyInput();
void doMouse(SDL_Event event);
void doMouse(SDL_Event);
void updateKeys(SDL_Event);
void updateJoystick(SDL_Event, int);
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
#endif
void joystickInit();
void inputInit();
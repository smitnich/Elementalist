#ifdef GEKKO
#include "wii.h"
#include <wupc/wupc.h>
#include <ogcsys.h>
#include <unistd.h>
int gcsensitivity = 80;
int padnum = 0;
u16 GCButtonsDown[4];
int GCInput(u16 GCButtonsDown);
int GCControlPort;
#include <wiikeyboard/keyboard.h>
extern bool showCursor;
u32 exp_type;
int WInput(u16,bool,int);
int classicInput(int);
int wupcInput(int wpadnum);
u16 WButtonsDown[4];
int wpadnum = 0;
bool mouseConnected = 0;
ir_t ir;
#include <wiikeyboard/keyboard.h>
#include <ogc/usbmouse.h>
#include <SDL/SDL.h>
#elif _WIN32
#include <SDL/SDL.h>
#endif
//#include <SDL/SDL_events.h>
bool useMouse = 0;
int pointerX = 0;
int pointerY = 0;
extern int numJoysticks;
int controltype = CONTROLLER_NONE;
bool checkGC();
int keyInput();
int secondaryControl;
int joyInput();
extern int chosenController;
extern int hatDirection;
struct mousePosition
{
	int x;
	int y;
};
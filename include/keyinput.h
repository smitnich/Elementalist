#include "base.h"
#ifdef GEKKO
#include <SDL/SDL.h>
#elif _WIN32
#include <sdl.h>
#elif LINUX
#include <SDL.h>
#endif
typedef enum
{
keyLeft,
keyRight,
keyUp,
keyDown,
altLeft,
altRight,
altUp,
altDown,
keyButton1,
keyButton2,
keyPrevLevel,
keyNextLevel,
altPrevLevel,
altNextLevel,
keyMenu,
keyMinimize
} keyControlDefs;
SDLKey keyControls[21] = {SDLK_a, SDLK_d, SDLK_w, SDLK_s, SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_z, SDLK_x, SDLK_MINUS, SDLK_EQUALS, SDLK_KP_MINUS, SDLK_KP_PLUS, SDLK_ESCAPE, SDLK_TAB};

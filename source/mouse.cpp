#ifdef GEKKO
#include <SDL/sdl.h>
#include <SDL/sdl_events.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#endif
extern int pointerX, pointerY;
//Update the mouse
void doMouse(SDL_Event mouseEvent)
{
	if (mouseEvent.type == SDL_MOUSEMOTION)
	{
		pointerX = mouseEvent.motion.x;
		pointerY = mouseEvent.motion.y;
	}
}

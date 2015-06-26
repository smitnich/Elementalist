#ifdef GEKKO
#include <SDL/sdl.h>
#include <SDL/sdl_events.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#endif
//Update the mouse
void checkClick(int x, int y);
int mouseX, mouseY;
void doMouse(SDL_Event mouseEvent)
{
	if (mouseEvent.type == SDL_MOUSEMOTION)
	{
		mouseX = mouseEvent.motion.x;
		mouseY = mouseEvent.motion.y;
	}
	if (mouseEvent.type == SDL_MOUSEBUTTONDOWN) {
		checkClick(mouseX, mouseY);
	}
}

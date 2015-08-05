#ifdef GEKKO
#include <SDL/sdl.h>
#include <SDL/sdl_events.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#endif
#include "inputdef.h"
//Update the mouse
void checkClick(int x, int y);
int mouseX, mouseY;
int mouseInput = INPUT_NONE;
extern bool showCursor;
void doMouse(SDL_Event mouseEvent)
{
	showCursor = true;
	if (mouseEvent.type == SDL_MOUSEMOTION)
	{
		mouseX = mouseEvent.motion.x;
		mouseY = mouseEvent.motion.y;
	}
	if (mouseEvent.type == SDL_MOUSEBUTTONDOWN) {
		if (mouseEvent.button.button == SDL_BUTTON_LEFT) {
			checkClick(mouseX, mouseY);
		}
		else if (mouseEvent.button.button == SDL_BUTTON_WHEELDOWN) {
			mouseInput = BUTTON_MOUSEWHEEL_DOWN;
		}
		else if (mouseEvent.button.button == SDL_BUTTON_WHEELUP) {
			mouseInput = BUTTON_MOUSEWHEEL_UP;
		}
	}
}

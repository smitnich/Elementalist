#include "sdlFiles.h"
#include "font.h"
#include "input.h"
#include "inputDef.h"
#include "gfx.h"
#include "event.h"

SDL_Surface *mainText[3] = {NULL, NULL, NULL };
SDL_Surface *musicText[2] = {NULL, NULL };
SDL_Surface *sdlText[2] = { NULL, NULL };
SDL_Surface *backText = NULL;

SDL_Surface *backdrop = NULL;

extern int lastInput;
extern SDL_Surface *screen;
// Make sure we stay open for at least a second
unsigned int startTime = 0;

SDL_Surface *makeTitleSurface(int width, int height, Uint32 color);


const char *sdlString[] = { "This game was made possible by the",
							"SDL library - www.libsdl.org" };
const char *mainString[] = { "Made by Nicholas Smith",
							 "Source code available at",
							 "github.com/smitnich/elementalist" };
const char *musicString[] = { "Music by Eric Matayas",
							  "www.soundimage.org" };

const char *backString = "Press any button to return";

extern int currentScreen;
void drawBackground();

void initCredits()
{
	for (int i = 0; i < 2; i++)
	{
		sdlText[i] = TTF_RenderText_Solid(fontSmall, sdlString[i], textColor);
		musicText[i] = TTF_RenderText_Solid(font, musicString[i], textColor);
	}
	mainText[0] = TTF_RenderText_Solid(font, mainString[0], textColor);
	for (int i = 1; i < 3; i++)
	{
		mainText[i] = TTF_RenderText_Solid(fontSmall, mainString[i], textColor);
	}
	backText = TTF_RenderText_Solid(fontSmall, backString, textColor);
	backdrop = makeTitleSurface(475, 340, SDL_MapRGB(screen->format, 192, 192, 192));
}
void drawCredits()
{
	drawBackground();
	apply_surface(75, 35, backdrop, screen);
	apply_surface(100, 50, mainText[0], screen);
	apply_surface(100, 75, mainText[1], screen);
	apply_surface(100, 100, mainText[2], screen);
	apply_surface(100, 150, sdlText[0], screen);
	apply_surface(100, 175, sdlText[1], screen);
	apply_surface(100, 250, musicText[0], screen);
	apply_surface(100, 300, musicText[1], screen);
	apply_surface(100, 350, backText, screen);
}
void doCredits()
{
	int input = INPUT_NONE;
	initCredits();
	currentScreen = SCR_CREDITS;
	startTime = SDL_GetTicks();
	do {
		checkEvents();
		drawCredits();
		drawMouse();
		SDL_Flip(screen);
		input = determineInput(false);
		if (input == lastInput)
			input = INPUT_NONE;
	} while (input == INPUT_NONE || (SDL_GetTicks() - 1000 <= startTime));
	for (int i = 0; i < 2; i++)
	{
		SDL_FreeSurface(mainText[i]);
		SDL_FreeSurface(sdlText[i]);
		SDL_FreeSurface(musicText[i]);
	}
	SDL_FreeSurface(mainText[2]);
	SDL_FreeSurface(backdrop);
	SDL_FreeSurface(backText);
}
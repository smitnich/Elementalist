#include "sdlFiles.h"
#include <SDL/SDL_ttf.h>
const int bufferSize = 128;
extern TTF_Font *fontSmall;
extern SDL_Color textColor;
char debugString[128];
SDL_Surface *debugText;
bool debugOn = false;

void writeDebugText(char* in)
{
	strncpy(debugString, in, bufferSize);
}
SDL_Surface *renderDebugText()
{
	if (debugText != NULL)
		SDL_FreeSurface(debugText);
	debugText = TTF_RenderText_Solid(fontSmall, debugString, textColor);
	return debugText;
}
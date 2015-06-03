#include "font.h"
#include <iostream>
//The font to show the level name in
TTF_Font *font = NULL;
//A smaller font
TTF_Font *fontSmall = NULL;
//Let the font be black
SDL_Color textColor = { 0, 0, 0 };
char fontpath[] = "fonts/imagine_font.ttf";
bool fontInit()
{
	TTF_Init();
	font = TTF_OpenFont( fontpath, 32 );
	fontSmall = TTF_OpenFont(fontpath,16);
	if (font == NULL || fontSmall == NULL)
		return 0;
	else
		return 1;
}
void fontCleanup()
{
	TTF_CloseFont(font);
	TTF_Quit();
}
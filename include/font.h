#include <SDL/SDL_ttf.h>
//Loads the fonts or returns false if they could not be opened
bool fontInit();
void fontCleanup();
extern TTF_Font *font;
extern TTF_Font *fontBig;
extern TTF_Font *fontSmall;
extern SDL_Color textColor;
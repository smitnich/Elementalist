#ifdef GEKKO
#include <SDL/SDL_ttf.h>
#elif _WIN32
#include <SDL_ttf.h>
#elif LINUX
#include <SDL/SDL_ttf.h>
#endif
//Loads the fonts or returns false if they could not be opened
bool fontInit();
void fontCleanup();
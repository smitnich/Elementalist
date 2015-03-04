#include "base.h"
#include "sdlFiles.h"
//The original declaration of all sprites
SDL_Surface *screen = NULL;
SDL_Surface *button = NULL;
SDL_Surface *tiles = NULL;
SDL_Surface *background = NULL;
SDL_Surface *selector = NULL;
SDL_Surface *personns[6] = { NULL };
SDL_Surface *personew[6] = { NULL };
SDL_Surface *deadPerson = NULL;
SDL_Surface *wall[47] = { NULL };
SDL_Surface *textBox[9] = { NULL };
SDL_Surface *text = NULL;
SDL_Surface *crate = NULL;
SDL_Surface *heavyCrate = NULL;
SDL_Surface *iceBlock = NULL;
SDL_Surface *cursor = NULL;
SDL_Surface *exitTile = NULL;
SDL_Surface *barrierTile = NULL;
SDL_Surface *pressureTile = NULL;
SDL_Surface *iceBallns[6] = { NULL };
SDL_Surface *iceBallew[6] = { NULL };
SDL_Surface *error = NULL;
SDL_Surface *conveyorew[6] = { NULL };
SDL_Surface *conveyorns[6] = { NULL };
SDL_Surface *iceFloor = NULL;
SDL_Surface *spr_bomb = NULL;
SDL_Surface *spr_pressureToggle[2] = { NULL, NULL };
SDL_Surface *spr_dupe[4] = { NULL, NULL, NULL, NULL };
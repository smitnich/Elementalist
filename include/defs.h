#ifndef defs
#define defs
#include "wallnames.h"

#define MAP_SIZE 48
enum directions_t
{
	D_NONE,
	D_UP,
	D_LEFT,
	D_DOWN,
	D_RIGHT
};
#define NUM_FRAMES 3
#define NUM_TILES 4
#define MAX_CHANNELS 2
#define BUFFERLENGTH 128
#define BOTTOM_IMAGE 0
#define OBJECT_IMAGE 1
#define TOP_IMAGE 2
#define NUM_TILE_IMAGES 3
#define TILE_SIZE 48
#define EMPTY 0
#define SOLID 1
#define MAX_LEVEL 14
enum screens_t
{
	SCR_LEVELSELECT,
	SCR_GAME
};
#endif
#ifndef defs
#define defs
#include "wallnames.h"
enum tiles_t
{ m_floor = 3,
m_wall = 4,
m_exit = 1,
m_barrier = 128,
m_pressure = 2,
m_conveyorn = 6,
m_conveyors = 7,
m_conveyorw = 8,
m_conveyore = 5,
m_any,
m_error
};

#define MAP_SIZE 48
enum directions_t
{
	D_NONE,
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT
};
enum objects_t
{
	OBJ_BLOCK = 9,
	OBJ_HEAVY_BLOCK = 10,
	OBJ_ICE_BLOCK = 11,
	OBJ_PLAYER = 12,
	OBJ_ICE_ELEMENT_E = 13,
	OBJ_ICE_ELEMENT_N = 14,
	OBJ_ICE_ELEMENT_S = 15,
	OBJ_ICE_ELEMENT_W = 16
};
#define MAX_LEVEL 5
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
#define PUSHABLE 2
#endif
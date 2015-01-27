#ifndef defs
#define defs
#include "wallnames.h"
enum tiles_t
{
m_exit = 1,
m_pressure,
m_floor,
m_wall,
m_conveyore,
m_conveyorn,
m_conveyors,
m_conveyorw,
m_barrier,
OBJ_BLOCK,
OBJ_HEAVY_BLOCK,
OBJ_ICE_BLOCK,
OBJ_PLAYER,
OBJ_ICE_ELEMENT_E,
OBJ_ICE_ELEMENT_N,
OBJ_ICE_ELEMENT_S,
OBJ_ICE_ELEMENT_W,
m_sender1,
m_receiver1,

m_any = 255,
m_error = 0
};
enum connectors_t
{

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
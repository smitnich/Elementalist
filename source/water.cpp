#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"

void addTerrainChange(int index, int changeTo);
SDL_Surface *spr_water = NULL;

Water::Water(int _index)
{
	index = _index;
	sprite = spr_water;
}
void Water::onEnter(Object *other)
{
	if (other->frozen)
		addTerrainChange(index, m_icefloor);
	else
		addTerrainChange(index, m_floor);
	if (!other->hovering)
		other->drown();
}
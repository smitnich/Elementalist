#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"

Terrain* addTerrainChange(int index, int changeTo);
SDL_Surface *spr_water = NULL;

Water::Water()
{
	index = 0;
	sprite = spr_water;
}
void Water::onEnter(Object *other)
{
	if (coveredTerrain)
		return;
	if (other->frozen)
		addTerrainChange(index, m_icefloor);
	else if (!other->isPlayer && !other->hovering)
		addTerrainChange(index, m_floor);
	if (!other->hovering)
		other->drown();
}
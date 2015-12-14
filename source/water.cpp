#include "SDLFiles.h"
#include "terrain.h"
#include "sprites.h"
#include "tileEnum.h"
#include "sound.h"

Terrain* addTerrainChange(int index, int changeTo);
SDL_Surface *spr_water = NULL;

Water::Water()
{
	index = 0;
	sprite = spr_water;
}
void Water::onEnter(Object *other, bool solid)
{
	if (solid)
		return;
	if (other->frozen)
		freeze();
	else if (!other->isPlayer && !other->hovering)
		addTerrainChange(index, m_floor);
	if (!other->hovering)
	{
		playSound(snd_splash);
		other->drown();
	}
}
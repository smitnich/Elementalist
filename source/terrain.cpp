#include "terrain.h"
#include "level.h"
#include "tileEnum.h"
void switchLevel(int levelNum);
bool playSound(Mix_Chunk *input);
extern int currentLevelNum;
extern SDL_Surface *wall[];
extern SDL_Surface *tiles;
extern SDL_Surface *exitTile;
extern SDL_Surface *barrierTile;
extern SDL_Surface *spr_bomb;
extern Mix_Chunk* snd_explode;
extern bool won;
Level *getCurrentLevel();
Terrain* instantiateTerrain(int newTerrain, int offset);
Floor::Floor()
{
	index = -1;
	isTrigger = false;
	this->sprite = tiles;
}
bool Wall::requestEntry(Object *other, int dir)
{
	return false;
}
bool Wall::isSolid()
{
	return true;
}
Wall::Wall()
{
	index = -1;
	isTrigger = false;
	this->sprite = wall[v_wallbase];
}
void Exit::onEnter(Object *other)
{
	if (other->isPlayer)
		won = true;
}
Exit::Exit()
{
	index = -1;
	isTrigger = false;
	this->sprite = exitTile;
}
Barrier::Barrier()
{
	index = 0;
	disabled = false;
	this->sprite = barrierTile;
}
void Barrier::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	if (!disabled)
		apply_surface(xStart, yStart, sprite, drawTo);
}
bool Barrier::requestEntry(Object *other, int dir)
{
	return disabled;
}
void Barrier::activate()
{
	disabled = true;
}
void Barrier::deactivate()
{
	disabled = false;
}
bool Barrier::isSolid()
{
	return !disabled;
}
Bomb::Bomb()
{
	index = 0;
	sprite = spr_bomb;
}
void swapTerrain(Terrain *orig, int newTerrain)
{
	int i = orig->index;
	delete(orig);
	orig = instantiateTerrain(newTerrain,i);
	getCurrentLevel()->mapLayer[i] = orig;
}
void Bomb::onEnter(Object *other)
{
	other->die();
	swapTerrain(this, m_floor);
	playSound(snd_explode);
}
void Bomb::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
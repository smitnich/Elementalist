#include "terrain.h"
#include "level.h"
#include "tileEnum.h"
#include <list>
void switchLevel(int levelNum);
bool playSound(Mix_Chunk *input);
unsigned char lookupWall(int index);
int reverseDir(int dir);
void doAssignQueue();
bool checkIfTerrainIsGlobal(Terrain *in);
extern int currentLevelNum;
SDL_Surface *wall[47] = { NULL };
SDL_Surface *tiles = NULL;
SDL_Surface *exitTile = NULL;
SDL_Surface *barrierTile = NULL;
SDL_Surface *spr_raisedFloor = NULL;
SDL_Surface *spr_bomb = NULL;
SDL_Surface *spr_colorBarrier[BARRIER_TYPES];
SDL_Surface *spr_bounceWall;
SDL_Surface *spr_risingWall;
int colorBlockIds[BARRIER_TYPES] = { OBJ_YELLOW_BLOCK };
extern Mix_Chunk *snd_explode;
extern bool won;
struct TerrainChangeRequest
{
	int index;
	int changeTo;
};
std::list<TerrainChangeRequest *> changeReqs;
Level *getCurrentLevel();
Terrain* instantiateTerrain(int newTerrain, int offset);
void addTerrainChange(int index, int changeTo)
{
	TerrainChangeRequest *req = new TerrainChangeRequest;
	req->index = index;
	req->changeTo = changeTo;
	changeReqs.push_back(req);
}
void swapTerrain(int index, int newTerrain)
{
	Terrain *terrain = instantiateTerrain(newTerrain, index);
	delete(getCurrentLevel()->mapLayer[index]);
	getCurrentLevel()->mapLayer[index] = terrain;
}
void doTerrainChanges()
{
	TerrainChangeRequest *req = NULL;
	while (!changeReqs.empty())
	{
		req = changeReqs.front();
		changeReqs.pop_front();
		swapTerrain(req->index, req->changeTo);
	}
}
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
Wall::Wall(int _index)
{
	index = _index;
	isTrigger = false;
	if (index < 0)
		sprite = wall[0];
	else
		sprite = wall[lookupWall(index)];
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
void ColorBarrier::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
ColorBarrier::ColorBarrier(int type)
{
	index = 0;
	colorType = type;
	sprite = spr_colorBarrier[type];
}
bool ColorBarrier::requestEntry(Object *other, int dir)
{
	//Need to check if the other object is a block of the right
	//color and delete this terrain as well as the block
	if (other->id == colorBlockIds[this->colorType])
	{
		return true;
	}
	return false;
}
void ColorBarrier::onEnter(Object *other)
{
	addTerrainChange(index, m_floor);
	other->die();
}
Bomb::Bomb()
{
	index = 0;
	sprite = spr_bomb;
}
void Bomb::onEnter(Object *other)
{
	other->die();
	addTerrainChange(index, m_floor);
	playSound(snd_explode);
}
void Bomb::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
RaisedFloor::RaisedFloor()
{
	sprite = spr_raisedFloor;
}
bool RaisedFloor::requestEntry(Object *other, int dir)
{
	if (other == NULL)
		return true;
	return !other->isMovableBlock();
}
BounceWall::BounceWall() {
	sprite = spr_bounceWall;
}
void BounceWall::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}

bool BounceWall::requestEntry(Object *other, int dir) {
	Terrain *terrain = getCurrentLevel()->getTerrain(other->x,other->y);
	other->prevMove = reverseDir(dir);
	terrain->onEnter(other);
	if (other->objMoveDir != dir)
		return true;
	else
		return false;
}
RisingWall::RisingWall(int _index) {
	sprite = spr_risingWall;
	index = _index;
}
void RisingWall::onExit(Object *other) {
	addTerrainChange(index, m_defaultWall);
}
void clearTerrain() {
	Level *curLevel = getCurrentLevel();
	if (curLevel == NULL)
		return;
	for (int i = 0; i < curLevel->height*curLevel->width; i++)
	{
		Terrain *tmp = curLevel->mapLayer.at(i);
		if (tmp != NULL && !checkIfTerrainIsGlobal(tmp))
			delete(tmp);
	}
}
#include "terrain.h"
#include "level.h"
#include "tileEnum.h"
#include <list>
void switchLevel(int levelNum);
bool playSound(Mix_Chunk *input);
void doAssignQueue();
void applyTerrain(int terrainNum, int index);
void addDeleteQueue(Terrain *in);
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
extern Mix_Chunk *snd_explode;
extern bool won;
struct TerrainChangeRequest
{
	int index;
	Terrain *changeTo;
};
std::list<TerrainChangeRequest> changeReqs;
Level *getCurrentLevel();
Terrain* instantiateTerrain(int newTerrain, int offset);
Terrain *addTerrainChange(int index, int changeTo)
{
	TerrainChangeRequest req;
	req.index = index;
	req.changeTo = instantiateTerrain(changeTo, index);
	changeReqs.push_back(req);
	return req.changeTo;
}
Terrain *addTerrainChange(int index, Terrain *newTerrain) {
	std::list<TerrainChangeRequest>::iterator it;
	for (it = changeReqs.begin(); it != changeReqs.end(); ++it) {
		if (it->index == index)
			return newTerrain;
	}
	TerrainChangeRequest req;
	req.index = index;
	req.changeTo = newTerrain;
	changeReqs.push_back(req);
	return newTerrain;
}
void swapTerrain(int index, Terrain *newTerrain)
{
	if (index != -1) {
		delete(getCurrentLevel()->mapLayer[index]);
		getCurrentLevel()->mapLayer[index] = newTerrain;
	}
}
void doTerrainChanges()
{
	TerrainChangeRequest req;
	while (!changeReqs.empty())
	{
		req = changeReqs.front();
		changeReqs.pop_front();
		swapTerrain(req.index, req.changeTo);
	}
}
void clearTerrain() {
	Level *curLevel = getCurrentLevel();
	if (curLevel == NULL)
		return;
	for (int i = 0; i < curLevel->height*curLevel->width; i++)
	{
		Terrain *tmp = curLevel->mapLayer.at(i);
		if (tmp != NULL)
			delete(tmp);
	}
}
Terrain::Terrain()
{
	coveredTerrain = false;
	isTrigger = false;
	solid = true;
}
Terrain::Terrain(SDL_Surface *sp)
{
	coveredTerrain = false;
	isTrigger = false;
	this->sprite = sp;
}
Terrain::~Terrain() {
}
void Terrain::freeze() {
	applyTerrain(m_icefloor,index);
}
void Terrain::heat() {
	return;
}
void Terrain::whileIn(Object *other) {
	return;
}
void Terrain::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, sprite, drawTo);
}
void Terrain::onDestroy()
{
	return;
}
void Terrain::onCreate()
{
	return;
}
void Terrain::activate()
{
	return;
}
void Terrain::deactivate()
{
	return;
}
bool Terrain::requestExit(Object* other, int dir)
{
	return true;
}
void Terrain::onEnter(Object* other, bool solidFound)
{
	return;
}
void Terrain::onExit(Object* other)
{
	return;
}
bool Terrain::requestEntry(Object* other, int dir)
{
	return true;
}
void applyTerrain(int input, int index) {
	MultipleTerrainManager *manager = NULL;
	Terrain *terrain = instantiateTerrain(input, index);
	terrain->index = index;
	Terrain *old = getCurrentLevel()->mapLayer.at(index);
	if (old != NULL && old->id != m_manager) {
		manager = (MultipleTerrainManager *) instantiateTerrain(m_manager, index);
		manager->addTerrain(old);
		manager->addTerrain(terrain);
		terrain = manager;
	}
	else if (old != NULL && old->id == m_manager) {
		manager = (MultipleTerrainManager *) getCurrentLevel()->mapLayer[index];
		manager->addTerrain(terrain);
		return;
	}
	getCurrentLevel()->mapLayer[index] = terrain;
}
void removeTerrain(Terrain *terrain) {
	int index = terrain->index;
	Terrain *tmp = getCurrentLevel()->mapLayer[index];
	if (tmp == terrain) {
		getCurrentLevel()->mapLayer[index] = NULL;
	}
	else if (tmp->id == m_manager) {
		MultipleTerrainManager* manager = (MultipleTerrainManager*) tmp;
		manager->removeTerrain(terrain);
	}
}
#include "terrain.h"
#include "level.h"
#include "tileEnum.h"
#include <list>
void switchLevel(int levelNum);
bool playSound(Mix_Chunk *input);
void doAssignQueue();
void applyTerrain(int terrainNum, int index);
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
void Terrain::onDestroyWrapper() {
	swapTerrain(this->index, within);
	within = NULL;
	onDestroy();
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
	within = NULL;
	coveredTerrain = false;
	isTrigger = false;
}
Terrain::Terrain(SDL_Surface *sp)
{
	within = NULL;
	coveredTerrain = false;
	isTrigger = false;
	this->sprite = sp;
}
Terrain::~Terrain() {
}
bool Terrain::requestEntryWrapper(Object *other, int dir) {
	return (requestEntry(other, dir) && (within == NULL || within->requestEntryWrapper(other, dir)));
}
bool Terrain::requestExitWrapper(Object *other, int dir) {
	return (requestExit(other, dir) && (within == NULL || within->requestEntryWrapper(other, dir)));
}
void Terrain::onEnterWrapper(Object *other) {
	onEnter(other);
	if (within != NULL)
		within->onEnterWrapper(other);
}
void Terrain::onExitWrapper(Object *other) {
	onExit(other);
	if (within != NULL)
		within->onExitWrapper(other);
}
void Terrain::activateWrapper() {
	activate();
	if (within != NULL)
		within->activate();
}
void Terrain::deactivateWrapper() {
	deactivate();
	if (within != NULL)
		within->activate();
}
void Terrain::drawWrapper(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff) {
	if (within != NULL)
		within->draw(drawTo, xTile, yTile, xOff, yOff);
	draw(drawTo, xTile, yTile, xOff, yOff);
}
void Terrain::whileInWrapper(Object *other) {
	whileIn(other);
	if (within != NULL)
		within->whileIn(other);
}
void Terrain::freezeWrapper() {
	freeze();
	if (within != NULL)
		within->freeze();
}
void Terrain::heatWrapper() {
	heat();
	if (within != NULL)
		within->heat();
}
void Terrain::freeze() {
	applyTerrain(m_icefloor,index);
}
void Terrain::heat() {
	return;
}
void Terrain::placeWithin(Terrain *terrain) {
	within = terrain;
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
void Terrain::onEnter(Object* other)
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
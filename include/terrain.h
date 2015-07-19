#ifndef _TERRAIN
#define _TERRAIN
#define MAX_CONNECTIONS 9
#define BARRIER_TYPES 1
#include "objectDef.h"
#include "defs.h"
#include <vector>
extern int xInitial;
extern int yInitial;
void apply_surface(int x, int y, SDL_Surface *apply, SDL_Surface *dest);
class Terrain
{
public:
	int id;
	bool isTrigger;
	bool coveredTerrain;
	int index;
	Terrain *within;
	SDL_Surface *sprite;
	Terrain()
	{
		within = NULL;
		coveredTerrain = false;
		isTrigger = false;
	}
	Terrain(SDL_Surface *sp)
	{
		within = NULL;
		coveredTerrain = false;
		isTrigger = false;
		this->sprite = sp;
	}
	virtual ~Terrain() {
	}
	void remove();
	bool requestEntryWrapper(Object *other, int dir) {
		return (requestEntry(other, dir) && (within == NULL || within->requestEntryWrapper(other, dir)));
	}
	bool requestExitWrapper(Object *other, int dir) {
		return (requestExit(other, dir) && (within == NULL || within->requestEntryWrapper(other, dir)));
	}
	void onEnterWrapper(Object *other) {
		onEnter(other);
		if (within != NULL)
			within->onEnterWrapper(other);
	}
	void onExitWrapper(Object *other) {
		onExit(other);
		if (within != NULL)
			within->onExitWrapper(other);
	}
	void activateWrapper() {
		activate();
		if (within != NULL)
			within->activate();
	}
	void deactivateWrapper() {
		deactivate();
		if (within != NULL)
			within->activate();
	}
	void onDestroyWrapper();

	void drawWrapper(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff) {
		if (within != NULL)
			within->draw(drawTo, xTile, yTile, xOff, yOff);
		draw(drawTo, xTile, yTile, xOff, yOff);
	}
	void whileInWrapper(Object *other) {
		whileIn(other);
		if (within != NULL)
			within->whileIn(other);
	}
	void freezeWrapper() {
		freeze();
		if (within != NULL)
			within->freeze();
	}
	void heatWrapper() {
		heat();
		if (within != NULL)
			within->heat();
	}
	void placeWithin(Terrain *terrain);
private:
	virtual void freeze();
	virtual void heat();
	virtual void whileIn(Object *other) {
		return;
	}
	virtual void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
	{
		int xStart = xTile*TILE_SIZE + xInitial + xOff;
		int yStart = yTile*TILE_SIZE + yInitial + yOff;
		apply_surface(xStart, yStart, sprite, drawTo);
	}
	virtual void onDestroy()
	{
		return;
	}
	virtual void onCreate()
	{
		return;
	}
	virtual void activate()
	{
		return;
	}
	virtual void deactivate()
	{
		return;
	}
	virtual bool requestExit(Object* other, int dir)
	{
		return true;
	}
	virtual void onEnter(Object* other)
	{
		return;
	}
	virtual void onExit(Object* other)
	{
		return;
	}
	virtual bool requestEntry(Object* other, int dir)
	{
		return true;
	}
};
class Floor : public Terrain{
public:
	Floor();
};
class RaisedFloor : public Terrain{
public:
	RaisedFloor();
	bool requestEntry(Object *other, int dir);
};
class Bomb : public Floor {
public:
	Bomb();
	void onEnter(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
};
class Wall : public Terrain{
public:
	bool requestEntry(Object* other, int dir);
	Wall(int index);
};
class Exit : public Floor{
public:
	void onEnter(Object* other);
	Exit();
};
class Barrier : public Terrain
{
private:
	bool disabled;
public:
	bool requestEntry(Object* other, int dir);
	void activate();
	void deactivate();
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	Barrier();
};
class ColorBarrier : public Terrain
{
public:
	int colorType;
	bool requestEntry(Object *other, int dir);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	void onEnter(Object *other);
	ColorBarrier(int type);
};
class Conveyor : public Terrain
{
private:
	double moveFraction;
	bool disabled;
	Object *lastEntered;
	int disableStartTime;
	int disabledTime;
public:
	int dir;
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	void activate();
	void deactivate();
	void whileIn(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	Conveyor();
	Conveyor(int direction);
};
extern std::vector<char> mapLayer;
class Trigger : public Terrain
{
public:
	std::vector<Terrain*> connections;
};
class PressureSwitch : public Trigger
{
public:
	void onEnter(Object* other);
	void onExit(Object* other);
	PressureSwitch();
};
class ToggleSwitch : public Trigger
{
public:
	bool enabled;
	void onEnter(Object *other);
	ToggleSwitch(bool enabled);
};
class IceFloor : public Terrain
{
public:
	~IceFloor();
	IceFloor(int index, Terrain *within);
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object *other);
	void onExit(Object *other);
	void activate();
	void deactivate();
	void whileIn(Object *other);
	void freeze();
	void heat();
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	IceFloor(Terrain *within);
};
class Water : public Terrain
{
public:
	void onEnter(Object *other);
	Water();
};
class Duplicator : public Terrain
{
public:
	int dir;
	bool createQueued;
	Object *copyObj;
	void onEnter(Object *other);
	void onExit(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	void activate();
	void deactivate();
	Duplicator(int dir);
};
class BlackHole : public Terrain
{
public:
	void onEnter(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	BlackHole();
};
class ElectricFloor : public Terrain
{
public:
	bool enabled;
	void onEnter(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	void activate();
	void deactivate();
	void whileIn(Object *other);
	ElectricFloor(bool _enabled);
};
class Freezer : public Terrain
{
public:
	Object *freezeObj;
	bool enabled;
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	void onEnter(Object *other);
	void onExit(Object *other);
	void activate();
	void deactivate();
	Freezer(bool _enabled);
};
class BounceWall : public Terrain {
public:
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	BounceWall();
	bool requestEntry(Object *other, int dir);
};
class RisingWall : public Terrain {
public:
	RisingWall(int index);
	void onExit(Object *other);
};
class OilFloor : public Terrain {
public:
	bool heated;
	bool objWithin;
	Terrain *within;
	~OilFloor();
	OilFloor(int index, Terrain *within);
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object *other);
	void onExit(Object *other);
	void activate();
	void deactivate();
	void whileIn(Object *other);
	void freeze();
	void heat();
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	OilFloor(Terrain *within);
};
#endif
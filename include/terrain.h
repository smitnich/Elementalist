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
	Terrain();
	Terrain(SDL_Surface *sp);
	virtual ~Terrain();
	void remove();
	bool requestEntryWrapper(Object *other, int dir);
	bool requestExitWrapper(Object *other, int dir);
	void onEnterWrapper(Object *other);
	void onExitWrapper(Object *other);
	void activateWrapper();
	void deactivateWrapper();
	void onDestroyWrapper();
	void drawWrapper(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	void whileInWrapper(Object *other);
	void freezeWrapper();
	void heatWrapper();
	void placeWithin(Terrain *terrain);
private:
	virtual void freeze();
	virtual void heat();
	virtual void whileIn(Object *other);
	virtual void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	virtual void onDestroy();
	virtual void onCreate();
	virtual void activate();
	virtual void deactivate();
	virtual bool requestExit(Object* other, int dir);
	virtual void onEnter(Object* other);
	virtual void onExit(Object* other);
	virtual bool requestEntry(Object* other, int dir);
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
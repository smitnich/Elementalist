#ifndef _TERRAIN
#define _TERRAIN
#define MAX_CONNECTIONS 9
#include "objectDef.h"
#include "base.h"
extern int xInitial;
extern int yInitial;
void apply_surface(int x, int y, SDL_Surface *apply, SDL_Surface *dest);
class Terrain
{
public:
	bool isTrigger;
	int index;
	SDL_Surface *sprite;
	Terrain()
	{
	}
	Terrain(SDL_Surface *sp)
	{
		this->sprite = sp;
	}
	virtual bool requestEntry(Object* other, int dir)
	{
		return true;
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
	virtual void activate()
	{
		return;
	}
	virtual void deactivate()
	{
		return;
	}
	virtual void onDestroy()
	{
		return;
	}
	virtual void onCreate()
	{
		return;
	}
	virtual void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
	{
		int xStart = xTile*TILE_SIZE + xInitial+xOff;
		int yStart = yTile*TILE_SIZE + yInitial+yOff;
		apply_surface(xStart, yStart, sprite, drawTo);
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
class Conveyor : public Terrain
{
private:
	double moveFraction;
	bool disabled;
	Object *lastEntered;
	Uint32 lastRender;
public:
	int dir;
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	void activate();
	void deactivate();
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	Conveyor();
	Conveyor(int direction);
};
extern vector<char> mapLayer;
class Trigger : public Terrain
{
public:
	vector<Terrain*> connections;
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
	void onEnter(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	IceFloor();
};
class Duplicator : public Terrain
{
public:
	int dir;
	Object *copyObj;
	void onEnter(Object *other);
	void onExit(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	void activate();
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
	ElectricFloor(bool _enabled);
};
#endif
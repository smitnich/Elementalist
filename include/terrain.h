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
	int index;
	SDL_Surface *sprite;
	Terrain()
	{
	}
	Terrain(SDL_Surface *sp)
	{
		this->sprite = sp;
	}
	virtual ~Terrain() {
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
	virtual void whileIn(Object *other) {
		return;
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
	unsigned int lastRender;
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
	void onEnter(Object *other);
	void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	IceFloor();
};
class Water : public Terrain
{
public:
	void onEnter(Object *other);
	Water(int _index);
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
#endif
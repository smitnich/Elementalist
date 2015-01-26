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
	virtual bool isSolid()
	{
		return false;
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
		return;
	}
};
class Floor : public Terrain{
public:
	Floor();
};
class Wall : public Terrain{
public:
	bool requestEntry(Object* other, int dir);
	bool isSolid();
	Wall();
};
class Exit : public Floor{
public:
	void onEnter(Object* other);
	Exit();
};
class Conveyor : public Terrain
{
private:
	bool disabled;
public:
	int dir;
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	bool isSolid();
	void activate();
	void deactivate();
	void onDestroy();
	void onCreate();
	//void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
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
	//void draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff);
	PressureSwitch();
};
#endif
#ifndef _TERRAIN
#define _TERRAIN
#define MAX_CONNECTIONS 9
#include "objectDef.h"
#include "base.h"
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
	virtual bool requestEntry(Object* other, int dir) = 0;
	virtual bool requestExit(Object* other, int dir) = 0;
	virtual void onEnter(Object* other) = 0;
	virtual void onExit(Object* other) = 0;
	virtual bool isSolid() = 0;
	virtual void activate() = 0;
	virtual void deactivate() = 0;
	virtual void onDestroy() = 0;
	virtual void onCreate() = 0;
};
class Floor : public Terrain{
public:
	Floor();
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	bool isSolid();
	void activate();
	void deactivate();
	void onDestroy();
	void onCreate();
};
class Wall : public Terrain{
public:
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	bool isSolid();
	void activate();
	void deactivate();
	void onDestroy();
	void onCreate();	
	Wall();
};
class Exit : public Floor{
public:
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	bool isSolid();
	void activate();
	void deactivate();
	void onDestroy();
	void onCreate();
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
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void onEnter(Object* other);
	void onExit(Object* other);
	bool isSolid();
	void activate();
	void deactivate();
	void onDestroy();
	void onCreate();
	PressureSwitch();
};
#endif
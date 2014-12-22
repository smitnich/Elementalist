#ifndef _TERRAIN
#define _TERRAIN
#include "objectDef.h"
#include "base.h"
class Terrain
{
public:
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
	virtual void OnEnter(Object* other) = 0;
	virtual void OnExit(Object* other) = 0;
	virtual bool isSolid() = 0;
	virtual void onActivate() = 0;
	virtual void onDeactivate() = 0;
	virtual void onDestroy() = 0;
	virtual void onCreate() = 0;
};
class Floor : public Terrain{
public:
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void OnEnter(Object* other);
	void OnExit(Object* other);
	bool isSolid();
	void onActivate();
	void onDeactivate();
	void onDestroy();
	void onCreate();
	Floor();
};
class Wall : public Terrain{
public:
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void OnEnter(Object* other);
	void OnExit(Object* other);
	bool isSolid();
	void onActivate();
	void onDeactivate();
	void onDestroy();
	void onCreate();
	Wall();
};
class Conveyor : public Terrain
{
public:
	int dir;
	bool requestEntry(Object* other, int dir);
	bool requestExit(Object* other, int dir);
	void OnEnter(Object* other);
	void OnExit(Object* other);
	bool isSolid();
	void onActivate();
	void onDeactivate();
	void onDestroy();
	void onCreate();
	Conveyor();
	Conveyor(int direction);
};
extern vector<char> mapLayer;
#endif
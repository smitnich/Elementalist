#include "base.h"
#include "sdlFiles.h"
#include "terrain.h"
#include "objectDef.h"
extern SDL_Surface *pressureTile;
void PressureSwitch::onEnter(Object* other)
{
	int length = connections.size();
	for (int i = 0; i < length; i++)
	{
		connections.at(i)->activate();
	}
}
void PressureSwitch::onExit(Object* other)
{
	int length = connections.size();
	for (int i = 0; i < length; i++)
	{
		connections.at(i)->deactivate();
	}
}
PressureSwitch::PressureSwitch()
{
	isTrigger = true;
	sprite = pressureTile;
}
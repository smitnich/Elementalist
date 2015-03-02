#include "base.h"
#include "sdlFiles.h"
#include "terrain.h"
#include "objectDef.h"
#include <list>
extern SDL_Surface *pressureTile;
extern SDL_Surface *spr_pressureToggle[2];
extern std::list<Trigger*> activateQueue;
void addActivateQueue(Trigger *in)
;
void PressureSwitch::onEnter(Object *other)
{
	int length = connections.size();
	for (int i = 0; i < length; i++)
	{
		connections.at(i)->activate();
	}
}
void PressureSwitch::onExit(Object *other)
{
	int length = connections.size();
	for (int i = 0; i < length; i++)
	{
		connections.at(i)->deactivate();
	}
}
PressureSwitch::PressureSwitch()
{
	index = 0;
	isTrigger = true;
	sprite = pressureTile;
}
void ToggleSwitch::onEnter(Object *other)
{
	enabled = !enabled;
	int length = connections.size();
	for (int i = 0; i < length; i++)
	{
		if (enabled)
			connections.at(i)->activate();
		else
			connections.at(i)->deactivate();
	}
	if (enabled)
		sprite = spr_pressureToggle[1];
	else
		sprite = spr_pressureToggle[0];
}
ToggleSwitch::ToggleSwitch(bool isEnabled)
{
	index = 0;
	isTrigger = true;
	if (isEnabled)
	{
		sprite = spr_pressureToggle[1];
		addActivateQueue(this);
	}
	else
	{
		sprite = spr_pressureToggle[0];
	}
	enabled = isEnabled;
}
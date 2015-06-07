#include "sdlFiles.h"
#include "terrain.h"
class Object;
#include <list>
SDL_Surface *spr_pressureToggle[2] = { NULL, NULL };
SDL_Surface *pressureTile = NULL;
extern std::list<Trigger*> activateQueue;
void addActivateQueue(Trigger *in);
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
	sprite = spr_pressureToggle[enabled];
}
ToggleSwitch::ToggleSwitch(bool isEnabled)
{
	enabled = isEnabled;
	index = 0;
	isTrigger = true;
	if (enabled)
		addActivateQueue(this);
	sprite = spr_pressureToggle[enabled];
}
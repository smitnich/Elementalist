#include "sdlFiles.h"
#include "terrain.h"
#include "level.h"

void teleportTo(Object *toTeleport, int dest, int start)
{
	Object *toDie = getCurrentLevel()->getObject(dest);
	if (toDie != NULL)
		toDie->die();
	int x, y;
	getCurrentLevel()->convertIndex(dest, x, y);
	toTeleport->x = x;
	toTeleport->y = y;
	getCurrentLevel()->assignObject(dest, toTeleport);
	getCurrentLevel()->assignObject(start, NULL);
}

SDL_Surface *spr_teleSource = NULL;
SDL_Surface *spr_teleDest = NULL;

Teleporter::Teleporter() : Trigger()
{
	sprite = spr_teleSource;
}
void Teleporter::onEnter(Object *in, bool solidFound)
{
	if (connections.size() == 0)
		return;
	int sendTo = connections.at(0).index;
	teleportTo(in, sendTo, index);
}
TeleDestination::TeleDestination() {
	sprite = spr_teleDest;
}
#include "objectDef.h"
#include "terrain.h"
#include "sprites.h"
#include "level.h"
SDL_Surface *spr_dupe[4] = { NULL, NULL, NULL, NULL };
void addCreationQueue(Object *in);

void Duplicator::onEnter(Object *other)
{
	copyObj = other;
	if (createQueued) {
		activate();
		createQueued = false;
	}
}
void Duplicator::onExit(Object *other)
{
	if (other == copyObj)
		copyObj = NULL;
}
void Duplicator::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff)
{
	int xStart = xTile*TILE_SIZE + xInitial + xOff;
	int yStart = yTile*TILE_SIZE + yInitial + yOff;
	apply_surface(xStart, yStart, tiles, drawTo);
	apply_surface(xStart, yStart, sprite, drawTo);
}
void Duplicator::activate()
{
	numConnectionsActive++;
	if (numConnectionsActive >= totalConnections) {
		if (copyObj == NULL) {
			createQueued = true;
			return;
		}
		int x = copyObj->x;
		int y = copyObj->y;
		if (dir == D_UP)
			y -= 1;
		else if (dir == D_DOWN)
			y += 1;
		else if (dir == D_LEFT)
			x -= 1;
		else if (dir == D_RIGHT)
			x += 1;
		Object *other = getCurrentLevel()->getObject(x, y);
		if (other != NULL && !other->requestEntry(copyObj, dir))
			return;
		Object *tmp = copyObj->clone(copyObj->x, copyObj->y);
		if (tmp == NULL)
			return;
		tmp->startMove(dir, 3);
		addCreationQueue(tmp);
	}
}
void Duplicator::deactivate() {
	numConnectionsActive--;
	copyObj = NULL;
	createQueued = false;
}
Duplicator::Duplicator(int _dir)
{
	totalConnections = numConnectionsActive = 0;
	dir = _dir;
	sprite = spr_dupe[dir - 1];
	index = 0;
	copyObj = NULL;
	createQueued = false;
}

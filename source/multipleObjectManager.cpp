#include "objectDef.h"
#include <vector>
#include "multipleObjectManager.h"


MultipleObjectManager::MultipleObjectManager(int _x, int _y) : Object() {
	x = _x;
	y = _y;
}
MultipleObjectManager::~MultipleObjectManager() {
	for (unsigned int i = 0; i < within.size(); i++) {
		delete within[i];
	}

}
void MultipleObjectManager::draw(int moveFractionX, int moveFractionY)
{
	for (unsigned int i = 0; i < within.size(); i++) {
		within[i]->draw(moveFractionX, moveFractionY);
	}
}
void MultipleObjectManager::addObject(Object *in) {
	within.push_back(in);
}
void MultipleObjectManager::doLogic() {
	for (std::vector<Object*>::iterator it = within.begin(); it != within.end(); it++) {
		double prevMove = (*it)->objMoveFraction;
		(*it)->update();
		bool moveCompleted = (prevMove > 0.0 && (*it)->objMoveFraction == 0);
		if (moveCompleted) {
			it = within.erase(it);
		}
	}
}
void MultipleObjectManager::die() {
	return;
}
void MultipleObjectManager::loadImages() {
	return;
}
SDL_Surface *MultipleObjectManager::getSprite() {
	return NULL;
}
bool MultipleObjectManager::requestEntry(Object *other, int dir) {
	for (unsigned int i = 0; i < within.size(); i++) {
		if (!within[i]->requestEntry(other, dir))
			return false;
	}
	return true;
}
void MultipleObjectManager::onEnter(Object *other, int _x, int _y) {
	for (unsigned int i = 0; i < within.size(); i++) {
		within[i]->onEnter(other, _x, _y);
	}
	within.push_back(other);
}
void MultipleObjectManager::onCollision(Object *other, int dir) {
	for (unsigned int i = 0; i < within.size(); i++) {
		within[i]->onCollision(other, dir);
	}
}
bool MultipleObjectManager::objMove() {
	return false;
}
Object *MultipleObjectManager::clone(int _x, int _y) {
	return NULL;
}
Object *MultipleObjectManager::createInstance(int _x, int _y) {
	return new MultipleObjectManager(_x, _y);
}
bool MultipleObjectManager::allowEntry() {
	return true;
}
MultipleObjectManager *makeObjectManager(int _x, int _y) {
	return new MultipleObjectManager(_x, _y);
}
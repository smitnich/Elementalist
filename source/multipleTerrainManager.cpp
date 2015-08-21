#include "terrain.h"
#include "tileEnum.h"

MultipleTerrainManager::MultipleTerrainManager() {
	id = m_manager;
}
void MultipleTerrainManager::freeze() {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->freeze();
	}
}
void MultipleTerrainManager::heat() {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->heat();
	}
}
void MultipleTerrainManager::whileIn(Object *other) {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->whileIn(other);
	}
}
void MultipleTerrainManager::draw(SDL_Surface *drawTo, int xTile, int yTile, int xOff, int yOff) {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->draw(drawTo, xTile, yTile, xOff, yOff);
	}
}
void MultipleTerrainManager::onDestroy() {
	for (unsigned int i = 0; i < within.size(); i++) {
		delete within.at(i);
	}

}
void MultipleTerrainManager::onCreate() {

}
void MultipleTerrainManager::addTerrain(Terrain *in) {
	within.push_back(in);
}
void MultipleTerrainManager::activate() {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->activate();
	}
}
void MultipleTerrainManager::deactivate() {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->deactivate();
	}
}
bool MultipleTerrainManager::requestExit(Object* other, int dir) {
	for (unsigned int i = 0; i < within.size(); i++) {
		if (!within.at(i)->requestExit(other, dir)) {
			return false;
		}
	}
	return true;

}
void MultipleTerrainManager::onEnter(Object* other) {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->onEnter(other);
	}
}
void MultipleTerrainManager::onExit(Object* other) {
	for (unsigned int i = 0; i < within.size(); i++) {
		within.at(i)->onExit(other);
	}
}
bool MultipleTerrainManager::requestEntry(Object* other, int dir) {
	for (unsigned int i = 0; i < within.size(); i++) {
		if (!within.at(i)->requestEntry(other, dir)) {
			return false;
		}
	}
	return true;
}
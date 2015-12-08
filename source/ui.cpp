#include "sdlFiles.h"
#include <list>
#include "defs.h"
#include "ui.h"
#include "level.h"
#include "levelSelect.h"
#include "title.h"

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
SDL_Surface* loadOptimizedIMG(const char *fileName);
extern int xInitial;
extern int yInitial;
extern int currentScreen;

struct InterfaceMember {
	int x, y;
	SDL_Surface *sprite;
	void (*doSomething)();
};
std::list<InterfaceMember> allMembers;

void createButtons() {
	const int margin = 16;
	InterfaceMember restart;
	restart.x = xInitial + TILE_SIZE*(NUM_TILES*2+1) + margin;
	restart.y = yInitial;
	restart.sprite = loadOptimizedIMG("gfx/ui/restartButton.png");
	restart.doSomething = &restartLevel;
	InterfaceMember nextLevelButton;
	nextLevelButton.x = xInitial + TILE_SIZE*(NUM_TILES * 2 + 1) + margin;
	nextLevelButton.y = (restart.y + restart.sprite->h) + margin;
	nextLevelButton.sprite = loadOptimizedIMG("gfx/ui/nextButton.png");
	nextLevelButton.doSomething = &nextLevel;
	InterfaceMember prevLevelButton;
	prevLevelButton.x = xInitial + TILE_SIZE*(NUM_TILES * 2 + 1) + margin;
	prevLevelButton.y = (nextLevelButton.y + nextLevelButton.sprite->h) + margin;
	prevLevelButton.sprite = loadOptimizedIMG("gfx/ui/prevButton.png");
	prevLevelButton.doSomething = &prevLevel;
	InterfaceMember levelSelectButton;
	levelSelectButton.x = xInitial + TILE_SIZE*(NUM_TILES * 2 + 1) + margin;
	levelSelectButton.y = (prevLevelButton.y + prevLevelButton.sprite->h) + margin;
	levelSelectButton.sprite = loadOptimizedIMG("gfx/ui/levelSelectButton.png");
	levelSelectButton.doSomething = &gotoLevelSelect;
	allMembers.push_front(restart);
	allMembers.push_front(nextLevelButton);
	allMembers.push_front(prevLevelButton);
	allMembers.push_front(levelSelectButton);
}

void renderUserInterface(SDL_Surface *screen) {
	for (std::list<InterfaceMember>::iterator it = allMembers.begin(); it != allMembers.end(); ++it)
	{
		apply_surface(it->x, it->y, it->sprite, screen);
	}
}
void checkClick(int x, int y) {
	switch (currentScreen) {
	case SCR_GAME:
		for (std::list<InterfaceMember>::iterator it = allMembers.begin(); it != allMembers.end(); ++it)
		{
			if (x >= it->x && x <= (it->x + it->sprite->w) && y >= it->y && y <= (it->y + it->sprite->h))
				it->doSomething();
		}
		break;
	case SCR_LEVELSELECT:
		handleLevelSelectClick(x, y);
		break;
	case SCR_TITLE:
		handleTitleClick(x, y);
		break;
	}
}
#include "sdlFiles.h"
#include <list>
#include "defs.h"

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
SDL_Surface* loadOptimizedIMG(const char *fileName);
extern int xInitial;
extern int yInitial;

void restartLevel();
void nextLevel();
void prevLevel();

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
	allMembers.push_front(restart);
	allMembers.push_front(nextLevelButton);
	allMembers.push_front(prevLevelButton);
}

void renderUserInterface(SDL_Surface *screen) {
	for (std::list<InterfaceMember>::iterator it = allMembers.begin(); it != allMembers.end(); ++it)
	{
		apply_surface(it->x, it->y, it->sprite, screen);
	}
}
void checkClick(int x, int y) {
	for (std::list<InterfaceMember>::iterator it = allMembers.begin(); it != allMembers.end(); ++it)
	{
		if (x >= it->x && x <= (it->x + it->sprite->w) && y >= it->y && y <= (it->y + it->sprite->h))
			it->doSomething();
	}
}
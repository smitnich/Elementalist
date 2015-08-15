#include "sdlFiles.h"
#include <list>
#include "defs.h"
#include "font.h"
#include "inputDef.h"
#include <string>

void checkEvents();
void drawSprite(int drawX, int drawY, SDL_Surface* toDraw);
int determineInput(bool mouse);
void switchLevel(int level);

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);

extern std::list<SDL_Surface *> allImages;

extern const std::string LevelStrings[];
extern TTF_Font *font;
extern int mouseX, mouseY;
extern int currentScreen;
extern SDL_Surface *screen;
SDL_Surface *spr_levelButton;
SDL_Surface *spr_levelButtonSelected;

extern SDL_Surface *cursor;

extern int videoSizeX;
extern int videoSizeY;

extern bool showCursor;

extern int levelChange;

SDL_Color defaultTextColor = { 0, 0, 0 };

int xMargin = 64;
int yMargin = 32;
int buttonSizeX = 256;
int buttonSizeY = 64;
int spacingY = 16;

bool finished = false;
extern int lastInput;
int selected = 0;

int scrollDistance = 0;

struct LevelButton {
	int x, y;
	SDL_Surface *text;
	int levelNum;
};
LevelButton allButtons[MAX_LEVEL];

bool checkWithin(LevelButton button, int x, int y) {
	int newY = y + scrollDistance*(buttonSizeY + spacingY);
	if ((x >= button.x) && x <= (button.x + spr_levelButton->w) && (newY >= button.y) && newY <= (button.y + spr_levelButton->h))
		return true;
	else
		return false;
}

SDL_Surface *renderText(const char *text, SDL_Color textColor) {
	return TTF_RenderText_Solid(font, text, textColor);
}

void makeLevelButtons() {
	int i;
	for (i = 1; i < MAX_LEVEL; i++) {
		LevelButton button;
		button.levelNum = i;
		button.x = xMargin;
		button.y = yMargin + (buttonSizeY + spacingY)*(i-1);
		button.text = renderText(LevelStrings[i].c_str(), defaultTextColor);
		allImages.push_front(button.text);
		allButtons[i] = button;
	}
}
void drawCenteredText(int xStart, int yStart, SDL_Surface *text, SDL_Surface *button) {
	int yMargin = yStart + (button->h-text->h)/2;
	int xMargin = xStart + (button->w - text->w) / 2;
	drawSprite(xMargin, yMargin, text);
}
void renderLevelSelectScreen() {
	int i;
	LevelButton tmpButton;
	SDL_Rect rect = { 0, 0, videoSizeX, videoSizeY };
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 91, 91, 255));
	for (i = 1; i < MAX_LEVEL; i++) {
		tmpButton = allButtons[i];
		if (allButtons[i].y - scrollDistance*(buttonSizeY + spacingY) >= videoSizeY){
			break;
		}
		if (i == selected || checkWithin(tmpButton,mouseX,mouseY)) {
			drawSprite(tmpButton.x, tmpButton.y - scrollDistance*(buttonSizeY + spacingY), spr_levelButtonSelected);
		}
		else {
			drawSprite(tmpButton.x, tmpButton.y - scrollDistance*(buttonSizeY + spacingY), spr_levelButton);
		}
		drawCenteredText(tmpButton.x, tmpButton.y - scrollDistance*(buttonSizeY + spacingY), tmpButton.text, spr_levelButton);
	}
	//Draw the mouse if it is within bounds and should be drawn
#ifdef GEKKO
	if (mouseX > -1 && mouseY > -1 && showCursor == true)
		apply_surface(mouseX - cursor->w / 2, mouseY, cursor, screen);
#endif
}
void checkInput() {
	int input = determineInput(true);
	if (input == lastInput)
		return;
	lastInput = input;
	switch (input) {
	case B_DOWN:
	case BUTTON_MOUSEWHEEL_DOWN:
		selected++;
		if (selected >= MAX_LEVEL) {
			selected = 1;
			scrollDistance = 0;
		}
		break;
	case B_UP:
	case BUTTON_MOUSEWHEEL_UP:
		selected--;
		if (selected < 1) {
			scrollDistance = MAX_LEVEL - 6;
			selected = MAX_LEVEL-1;
		}
		break;
	case BUTTON_1:
		if (selected != 0)
			finished = true;
		break;
	default:
		break;
	}
	if (scrollDistance < selected-5) {
		scrollDistance += 1;
	}
	else if (scrollDistance >= selected) {
		scrollDistance -= 1;
	}
}
int selectLevel() {
	currentScreen = SCR_LEVELSELECT;
	finished = false;
	while (!finished) {
		renderLevelSelectScreen();
		checkEvents();
		checkInput();
		SDL_Flip(screen);
	}
	currentScreen = SCR_GAME;
	return selected;
}
void handleLevelSelectClick(int x, int y) {
	int i;
	for (i = 1; i < MAX_LEVEL; i++) {
		if (checkWithin(allButtons[i], x, y)) {
			selected = i;
			finished = true;
			return;
		}
	}
}
void gotoLevelSelect() {
	levelChange = selectLevel();
}
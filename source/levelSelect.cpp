#include "sdlFiles.h"
#include <list>
#include "defs.h"
#include "font.h"
#include "inputDef.h"
#include <string>

#include "levelSelect.h"
#include "gfx.h"
#include "event.h"
#include "input.h"
#include "level.h"
#include "main.h"
#include "sprites.h"
#include "sound.h"

extern int bitDepth;
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

static int borderMargin = 5;
static int textMargin = 10;

int scrollDistance = 1;

struct LevelButton {
	int x, y;
	int xMargin, yMargin;
	SDL_Surface *text;
	SDL_Surface *backdrop[2];
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
#define GREEN 34,177,76
#define YELLOW 255,242,0
	for (i = 1; i < MAX_LEVEL; i++) {
		LevelButton button;
		button.levelNum = i;
		button.x = xMargin;
		button.y = yMargin + (buttonSizeY + spacingY)*(i-1);
		button.text = renderText(LevelStrings[i].c_str(), defaultTextColor);
		button.backdrop[0] = renderBackground(button.text->w + textMargin*2, GREEN);
		button.backdrop[1] = renderBackground(button.text->w + textMargin*2, YELLOW);
		allImages.push_front(button.text);
		allButtons[i] = button;
	}
}

SDL_Surface *makeSurface(int width, int height)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(0, width + borderMargin, height + borderMargin,
											screen->format->BitsPerPixel, 0, 0, 0, 0);
	allImages.push_front(surf);
	return surf;
}

SDL_Surface *renderBackground(int width, int r, int g, int b)
{
	static const int height = 48 + textMargin*2;
	SDL_Surface *surf = makeSurface(width, height);
	SDL_Rect borderRect = { 0, 0, width, height };
	SDL_Rect innerRect = { borderMargin, borderMargin, width - borderMargin, height - borderMargin};
	SDL_FillRect(surf, &borderRect, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_FillRect(surf, &innerRect, SDL_MapRGB(screen->format, r, g, b));
	return surf;
}

void drawCenteredText(int xStart, int yStart, SDL_Surface *text, SDL_Surface *button) {
	int yMargin = yStart + (button->h-text->h)/2;
	int xMargin = xStart + (button->w - text->w) / 2;
	drawSprite(xMargin, yMargin, text);
}
void drawBackground()
{
	for (int x = 0; x <= screen->w / TILE_SIZE; x++)
	{
		for (int y = 0; y <= screen->h / TILE_SIZE; y++)
		{
			apply_surface(x*TILE_SIZE, y*TILE_SIZE, tiles, screen);
		}
	}
}
void renderLevelSelectScreen() {
	int i;
	LevelButton tmpButton;
	// Text doesn't quite align in the center properly yet, so we add this to its
	// y value
	static int yOffset = 3;
	const int scrollOffset = scrollDistance*(buttonSizeY + spacingY);
	SDL_Rect rect = { 0, 0, videoSizeX, videoSizeY };
	//SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 91, 91, 255));
	drawBackground();
	for (i = 1; i < MAX_LEVEL; i++) {
		tmpButton = allButtons[i];
		if (allButtons[i].y - scrollOffset >= videoSizeY){
			break;
		}
		if (i == selected || checkWithin(tmpButton,mouseX,mouseY)) {
			drawSprite(tmpButton.x, tmpButton.y - scrollOffset, tmpButton.backdrop[1]);
		}
		else {
			drawSprite(tmpButton.x, tmpButton.y - scrollOffset, tmpButton.backdrop[0]);
		}
		int xPos = tmpButton.x + borderMargin + textMargin;
		int yPos = tmpButton.y + tmpButton.text->h / 2 + yOffset
				   - scrollDistance*(buttonSizeY + spacingY) + textMargin;
		drawSprite(xPos, yPos, tmpButton.text);
		//drawCenteredText(tmpButton.x, tmpButton.y - scrollDistance*(buttonSizeY + spacingY), tmpButton.text, spr_levelButton);
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
			scrollDistance = 1;
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
	case BUTTON_2:
		if (selected != 0)
			finished = true;
		break;
	case BUTTON_MENU:
		cleanup();
		break;
	default:
		return;
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
// Currently the level select music doesn't play nicely with
// the Wii version; so its disabled currently
#ifndef GEKKO
	Mix_PlayMusic(levelSelectMusic, -1);
#endif
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
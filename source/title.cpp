#include "sdlFiles.h"
#include "font.h"
#include "gfx.h"
#include "sound.h"
#include "input.h"
#include "event.h"
#include "main.h"

extern int mouseX, mouseY;

SDL_Surface *titleText = NULL;
SDL_Surface *startText = NULL;
SDL_Surface *creditsText = NULL;
SDL_Surface *quitText = NULL;

static int selected = 0;
static const int numButtons = 3;

void drawBackground();
void doCredits();

extern std::list<SDL_Surface *> allImages;

SDL_Color titleTextColor = {255, 255, 255};

SDL_Surface *titleBackground = NULL;
SDL_Surface *backgroundSurface[2][numButtons];
static const int borderMargin = 5;
static Uint32 selectedColor[2] = { 0, 0 };
SDL_Rect buttonRects[numButtons] = { 0 };

bool exitRequested = false;

SDL_Surface *makeTitleSurface(int width, int height, Uint32 color)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(0, width + borderMargin, height + borderMargin,
		screen->format->BitsPerPixel, 0, 0, 0, 0);
	allImages.push_front(surf);
	SDL_Rect rect = { borderMargin, borderMargin, width-borderMargin, height-borderMargin};
	SDL_FillRect(surf, NULL, 0x000000);
	SDL_FillRect(surf, &rect, color);
	return surf;
}

void initTitle()
{
	titleText = TTF_RenderText_Solid(fontBig, "The Elemental Factory", titleTextColor);
	startText = TTF_RenderText_Solid(font, "Start", titleTextColor);
	creditsText = TTF_RenderText_Solid(font, "Credits", titleTextColor);
	quitText = TTF_RenderText_Solid(font, "Quit", titleTextColor);
	selectedColor[0] = SDL_MapRGB(screen->format, 192, 192, 192);
	selectedColor[1] = SDL_MapRGB(screen->format, 255, 242, 0);
	titleBackground = makeTitleSurface(titleText->w+10, titleText->h+10, selectedColor[0]);
	for (int i = 0; i < 2; i++)
	{
		backgroundSurface[i][0] = makeTitleSurface(startText->w+10, startText->h+10, selectedColor[i]);
		backgroundSurface[i][1] = makeTitleSurface(creditsText->w+10, creditsText->h+10, selectedColor[i]);
		backgroundSurface[i][2] = makeTitleSurface(quitText->w+10, quitText->h+10, selectedColor[i]);
	}
	playMusic(titleMusic);
}

void drawTitle()
{
	drawBackground();
	int tmp = getCenter(screen->w, titleText->w);
	apply_surface(tmp - borderMargin, 25 - borderMargin, titleBackground, screen);
	apply_surface(tmp, 30, titleText, screen);
	tmp = getCenter(screen->w, startText->w);
	buttonRects[0] = { tmp - borderMargin, 150 - borderMargin, backgroundSurface[0][0]->w, backgroundSurface[0][0]->h };
	apply_surface(tmp - borderMargin, 150 - borderMargin, backgroundSurface[selected == 0][0], screen);
	apply_surface(tmp, 155, startText, screen);
	tmp = getCenter(screen->w, creditsText->w);
	buttonRects[1] = { tmp - borderMargin, 275 - borderMargin, backgroundSurface[0][1]->w, backgroundSurface[0][1]->h };
	apply_surface(tmp - borderMargin, 275 - borderMargin, backgroundSurface[selected == 1][1], screen);
	apply_surface(tmp, 280, creditsText, screen);
	tmp = getCenter(screen->w, quitText->w);
	buttonRects[2] = { tmp - borderMargin, 400 - borderMargin, backgroundSurface[0][2]->w, backgroundSurface[0][2]->h };
	apply_surface(tmp - borderMargin, 400 - borderMargin, backgroundSurface[selected == 2][2], screen);
	apply_surface(tmp, 405, quitText, screen);
	SDL_Flip(screen);
}
// Check if the mouse is within any buttons
int checkButtons(int x, int y)
{
	for (int i = 0; i < numButtons; i++)
	{
		SDL_Rect tmp = buttonRects[i];
		if (x >= tmp.x && x <= tmp.x + tmp.w && y >= tmp.y && y <= tmp.y + tmp.h)
			return i;
	}
	return -1;
}
void buttonSelected(int which)
{
	switch (selected)
	{
	case 0:
		exitRequested = true;
		break;
	case 1:
		doCredits();
		break;
	default:
		break;
	}
}
void handleTitleClick(int x, int y)
{
	int tmp = checkButtons(x, y);
	if (tmp != -1)
		selected = tmp;
	else
		return;
	if (tmp == 2)
	{
		exitRequested = true;
		cleanup();
	}
	else
	{
		buttonSelected(tmp);
	}
}

void titleLoop()
{
	static int lastInput = INPUT_NONE;
	static int input = INPUT_NONE;
	initTitle();
	currentScreen = SCR_TITLE;
	while (!exitRequested)
	{
		drawTitle();
		checkEvents();
		drawMouse();
		input = determineInput(true);
		if (input == lastInput)
			continue;
		lastInput = input;
		switch (input)
		{
		case BUTTON_MENU:
			cleanup();
			break;
		case BUTTON_MOUSEWHEEL_DOWN:
		case B_DOWN:
			selected = (selected + 1) % numButtons;
			break;
		case BUTTON_MOUSEWHEEL_UP:
		case B_UP:
			selected = (selected - 1);
			if (selected < 0)
				selected += numButtons;
			break;
		// Fall through after setting the selected object
		case BUTTON_1:
		case BUTTON_2:
			buttonSelected(selected);
			break;
		}
	}
}
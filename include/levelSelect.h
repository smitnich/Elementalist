bool checkWithin(struct LevelButton button, int x, int y);
SDL_Surface *renderText(const char *text, SDL_Color textColor);
void makeLevelButtons();
void drawCenteredText(int xStart, int yStart, SDL_Surface *text, SDL_Surface *button);
void renderLevelSelectScreen();
void checkInput();
int selectLevel();
void handleLevelSelectClick(int x, int y);
void gotoLevelSelect();
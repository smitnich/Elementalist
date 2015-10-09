#include "sdlFiles.h"
#include "inputDef.h"
#include "font.h"
#include "objectDef.h"
#include "defs.h"
void doWall(int type,int x,int y, int moveFractionX, int moveFractionY);
void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY);
int getCenter(int a, int b);
int getControlType();
void gfxInit();
SDL_Rect* rectMake(int x, int y, int h, int w);
SDL_Surface* loadOptimizedIMG(std::string);
void changeTextToDead();
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
void apply_surface(int x, int y, int width, int height, SDL_Surface* source, SDL_Surface* destination);
void objectDraw();
void doTextBox(int posY);
void drawSprite(int, int, SDL_Surface*);
void imgInit();
void drawWrappedSprite(int x, int y, SDL_Surface* source, SDL_Surface* destination, int xWrap, int yWrap);
void drawBorders();
void drawScreen();
#include "base.h"
#include "sdlFiles.h"
#include "input_def.h"
#include "font.h"
#include "objectDef.h"
void doWall(int type,int x,int y, int moveFractionX, int moveFractionY);
void calculateMoveFraction(int moveDir, int moveFraction, int *moveFractionX, int *moveFractionY);
void doDraw(Object *drawObject, int xOffset, int yOffset);
int getCenter(int a, int b);
int getControlType();
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
void apply_surface(int x, int y, int width, int height, SDL_Surface* source, SDL_Surface* destination);
void drawMe();
void objectDraw();
void doTextBox(int posY);
void drawSprite(int,int,SDL_Surface*);
void imgInit();
void drawWrappedSprite(int x, int y, SDL_Surface* source, SDL_Surface* destination, int xWrap, int yWrap);
double getPlayerMoveFraction();
int getPlayerMoveDir();
void drawBorders();
SDL_Rect* rectMake(int x, int y, int h, int w);
SDL_Surface* loadOptimizedIMG(string);
extern int lastMoveDir;
extern int videoSizeY,videoSizeX;
extern int xInitial, yInitial, tilesSizeX, tilesX, tilesY, tileSizeY;
extern int pointerX, pointerY, currentLevelNum;
extern string appPath;
extern string levelPath;
extern string startLevelName;
extern char map[MAP_SIZE][MAP_SIZE];
extern int varMap[MAP_SIZE][MAP_SIZE];
extern int pressureCount;
extern int totalPressureCount;
extern bool displayName;
extern int framesPerSecond;
extern int xMax, yMax;
#include "level.h"
#include "sprites.h"
extern bool showCursor;
extern int frame;
//The font to show the level name in
extern TTF_Font *font;
//A smaller font
extern TTF_Font *fontSmall;
//Let the font be black
extern SDL_Color textColor;
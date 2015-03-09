#include "base.h"
#include "sdlFiles.h"
#include "sprites.h"
#define COLORKEY 0xFF00DC
extern string appPath;
SDL_Surface* loadOptimizedIMG(string fileName);

#include <list>
struct TerrainChangeRequest;
extern std::list<TerrainChangeRequest *> changeReqs;

//Load a whole bunch of images
//Todo: Merge all image files into one and blit the portions of it onto individual sprites
void imgInit()
{
	int ignore = changeReqs.size();
	tiles = loadOptimizedIMG("gfx/tile.png");
	selector = loadOptimizedIMG("gfx/selector.png");
	crate = loadOptimizedIMG("gfx/block.png");
	heavyCrate = loadOptimizedIMG("gfx/heavyBlock.png");
	iceBlock = loadOptimizedIMG("gfx/iceBlock.png");
	personns[0] = loadOptimizedIMG("gfx/personn1.png");
	personns[1] = loadOptimizedIMG("gfx/personn2.png");
	personns[2] = loadOptimizedIMG("gfx/personn3.png");
	personns[3] = loadOptimizedIMG("gfx/persons1.png");
	personns[4] = loadOptimizedIMG("gfx/persons2.png");
	personns[5] = loadOptimizedIMG("gfx/persons3.png"); 
	personew[0] = loadOptimizedIMG("gfx/personw1.png");
	personew[1] = loadOptimizedIMG("gfx/personw2.png");
	personew[2] = loadOptimizedIMG("gfx/personw3.png");
	personew[3] = loadOptimizedIMG("gfx/persone1.png");
	personew[4] = loadOptimizedIMG("gfx/persone2.png");
	personew[5] = loadOptimizedIMG("gfx/persone3.png");
	deadPerson = loadOptimizedIMG("gfx//persondead.png");
	wall[0] = loadOptimizedIMG("gfx/wall/wallbase.png");
	wall[1] = loadOptimizedIMG("gfx/wall/wall1.png");
	wall[2] = loadOptimizedIMG("gfx/wall/wall2.png");
	wall[3] = loadOptimizedIMG("gfx/wall/wall3.png");
	wall[4] = loadOptimizedIMG("gfx/wall/wall4.png");
	wall[5] = loadOptimizedIMG("gfx/wall/wall5.png");
	wall[6] = loadOptimizedIMG("gfx/wall/wall6.png");
	wall[7] = loadOptimizedIMG("gfx/wall/wall7.png");
	wall[8] = loadOptimizedIMG("gfx/wall/wall8.png");
	wall[9] = loadOptimizedIMG("gfx/wall/wall9.png");
	wall[10] = loadOptimizedIMG("gfx/wall/wall10.png");
	wall[11] = loadOptimizedIMG("gfx/wall/wall11.png");
	wall[12] = loadOptimizedIMG("gfx/wall/wall12.png");
	wall[13] = loadOptimizedIMG("gfx/wall/wall13.png");
	wall[14] = loadOptimizedIMG("gfx/wall/wall14.png");
	wall[15] = loadOptimizedIMG("gfx/wall/wall15.png");
	wall[16] = loadOptimizedIMG("gfx/wall/wall16.png");
	wall[17] = loadOptimizedIMG("gfx/wall/walle.png");
	wall[18] = loadOptimizedIMG("gfx/wall/wallew.png");
	wall[19] = loadOptimizedIMG("gfx/wall/wallw.png");
	wall[20] = loadOptimizedIMG("gfx/wall/wallewn1.png");
	wall[21] = loadOptimizedIMG("gfx/wall/wallewn2.png");
	wall[22] = loadOptimizedIMG("gfx/wall/wallewn3.png");
	wall[23] = loadOptimizedIMG("gfx/wall/wallewn4.png");
	wall[24] = loadOptimizedIMG("gfx/wall/walls.png");
	wall[25] = loadOptimizedIMG("gfx/wall/wallse1.png");
	wall[26] = loadOptimizedIMG("gfx/wall/wallse2.png");
	wall[27] = loadOptimizedIMG("gfx/wall/wallsw1.png");
	wall[28] = loadOptimizedIMG("gfx/wall/wallsw2.png");
	wall[29] = loadOptimizedIMG("gfx/wall/walln.png");
	wall[30] = loadOptimizedIMG("gfx/wall/wallesn1.png");
	wall[31] = loadOptimizedIMG("gfx/wall/wallesn2.png");
	wall[32] = loadOptimizedIMG("gfx/wall/wallesn3.png");
	wall[33] = loadOptimizedIMG("gfx/wall/wallesn4.png");
	wall[34] = loadOptimizedIMG("gfx/wall/wallwsn1.png");
	wall[35] = loadOptimizedIMG("gfx/wall/wallwsn2.png");
	wall[36] = loadOptimizedIMG("gfx/wall/wallwsn3.png");
	wall[37] = loadOptimizedIMG("gfx/wall/wallwsn4.png");
	wall[38] = loadOptimizedIMG("gfx/wall/wallns.png");
	wall[39] = loadOptimizedIMG("gfx/wall/wallsew1.png");
	wall[40] = loadOptimizedIMG("gfx/wall/wallsew2.png");
	wall[41] = loadOptimizedIMG("gfx/wall/wallsew3.png");
	wall[42] = loadOptimizedIMG("gfx/wall/wallsew4.png");
	wall[43] = loadOptimizedIMG("gfx/wall/wallnw1.png");
	wall[44] = loadOptimizedIMG("gfx/wall/wallnw2.png");
	wall[45] = loadOptimizedIMG("gfx/wall/wallne1.png");
	wall[46] = loadOptimizedIMG("gfx/wall/wallne2.png");
	textBox[0] = loadOptimizedIMG("gfx/text/textbase.png");
	textBox[1] = loadOptimizedIMG("gfx/text/texte.png");
	textBox[2] = loadOptimizedIMG("gfx/text/textn.png");
	textBox[3] = loadOptimizedIMG("gfx/text/textne.png");
	textBox[4] = loadOptimizedIMG("gfx/text/textnw.png");
	textBox[5] = loadOptimizedIMG("gfx/text/texts.png");
	textBox[6] = loadOptimizedIMG("gfx/text/textse.png");
	textBox[7] = loadOptimizedIMG("gfx/text/textsw.png");
	textBox[8] = loadOptimizedIMG("gfx/text/textw.png");
	cursor = loadOptimizedIMG("gfx/cursor_icon.png");
	exitTile = loadOptimizedIMG("gfx/exit.png");
	barrierTile = loadOptimizedIMG("gfx/wall/barrier.png");
	pressureTile = loadOptimizedIMG("gfx/pressure.png");
	iceFloor = loadOptimizedIMG("gfx/icefloor.png");
	spr_bomb = loadOptimizedIMG("gfx/bomb.png");
	spr_pressureToggle[1] = loadOptimizedIMG("gfx/pressureToggleOff.png");
	spr_pressureToggle[0] = loadOptimizedIMG("gfx/pressureToggleOn.png");
	//Temporary graphics
	conveyor[D_LEFT-1] = loadOptimizedIMG("gfx/conveyor/conveyorw1.png");
	conveyor[D_UP-1] = loadOptimizedIMG("gfx/conveyor/conveyorn1.png");
	conveyor[D_RIGHT-1] = loadOptimizedIMG("gfx/conveyor/conveyore1.png");
	conveyor[D_DOWN-1] = loadOptimizedIMG("gfx/conveyor/conveyors1.png");
	error = loadOptimizedIMG("gfx/iamerror.png");
	spr_dupe[0] = loadOptimizedIMG("gfx/dupeN.png");
	spr_dupe[1] = loadOptimizedIMG("gfx/dupeS.png");
	spr_dupe[2] = loadOptimizedIMG("gfx/dupeW.png");
	spr_dupe[3] = loadOptimizedIMG("gfx/dupeE.png");
	spr_blackHole = loadOptimizedIMG("gfx/blackHole.png");
	spr_raisedFloor = loadOptimizedIMG("gfx/raisedTile.png");
	spr_electricFloor[0] = loadOptimizedIMG("gfx/electricFloorOff.png");
	spr_electricFloor[1] = loadOptimizedIMG("gfx/electricFloor.png");
	spr_freezer[1] = loadOptimizedIMG("gfx/freezerFloor.png");
	spr_freezer[0] = loadOptimizedIMG("gfx/freezerFloorOff.png");
	spr_pickup = loadOptimizedIMG("gfx/pickup.png");
	spr_pickupWall = loadOptimizedIMG("gfx/pickupWall.png");
	spr_water = loadOptimizedIMG("gfx/water.png");
}
//Optimize the image for proper depth and for transparecny
SDL_Surface* loadOptimizedIMG(string fileName)
{
	SDL_Surface *loadedImage = IMG_Load(fileName.c_str());
	if (loadedImage == NULL)
	{
		exit(0);
	}
	SDL_Surface *newImage = SDL_DisplayFormat( loadedImage );
	Uint32 colorkey = SDL_MapRGB( newImage->format, 0xFF, 0x00, 0xDC );
	SDL_SetColorKey(newImage, SDL_SRCCOLORKEY, colorkey );
	SDL_FreeSurface(loadedImage);
	return newImage;
}

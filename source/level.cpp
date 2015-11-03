#include "sdlFiles.h"
#include "level.h"
#include "objectDef.h"
#include "terrain.h"
#include "tileEnum.h"
#include "wallLookup.h"
#include <queue>
#include "queues.h"
#include "sound.h"
#include "object.h"

#include "level.h"

const std::string LevelStrings[] =
{
	"Dummy",//0
	"Conveyors",//1
	"The Sea",//2
	"Electricity",//3
	"Magnets",//4
	"Pipes",
	"Bombs",
	"Frozen",
	"Ice Blocks",
	"Flooded Prison",
	"One Way",
	"Toggle Trick",
	"Teleporters",
	"Duplicators"
};

void doTextBox(int);

//The number of objects created
extern unsigned int numObjects;
const char* header = "[header]";
const char* layer = "[layer]";
extern bool playerPlaced;
bool mapLoaded[MAX_LEVEL] = { 0 };
//Which level we are on
int levelNum = 1;
//The path to the level
extern std::string levelPath;
//The name of the starting level
extern std::string startLevelName;
//The last time to have user input
extern unsigned long lastInputTime;
//The last movement of the player
extern int prevMove;
//Whether to display the level name
extern bool displayName;
unsigned long levelStartTime;
//Whether or not the player has won the level
extern bool won;
extern int frame;
extern bool replayEnabled;
extern Mix_Music* levelMusic[MAX_LEVEL];
class Terrain *instantiateTerrain(int input, int offset);
extern int currentLevelNum;
extern Object* player;
void changeText();
unsigned long getTicks();

void resetMoves();
void dumpMoves();
void setTicks();
void clearLogicList();

Object* objectInit(unsigned int id, int x, int y);
std::string constructLevelName(int);
class Level *allLevels[MAX_LEVEL];
Level::Level(FILE* inFile, int levelNum)
{
	pickupCount = 0;
	allLevels[levelNum] = this;
	char buffer[128] = {0};
	fgets(buffer,128,inFile);
	if (strncmp(buffer,header,strlen(header)) != 0)
	{
		return;
	}
	std::string name;
	std::string value;
	do 
	{
		fgets(buffer,128,inFile);
		std::string str = std::string(buffer);
		size_t split = str.find("=");
		if (split == std::string::npos)
			continue;
		name = (str.substr(0,split));
		value = (str.substr(split+1,std::string::npos));
		if (name == "width")
		{
			width = atoi(value.c_str());
		}
		else if (name == "height")
		{
			height = atoi(value.c_str());
		}
		else if (name == "tileheight" || name == "tilewidth" || name == "orientation")
		{
			//Who cares, we don't need this information currently
		}
	} while (buffer[0] != '[' || buffer[1] != 'l');
	loadAllLayers(buffer,inFile);
}
void Level::loadAllLayers(char *buffer,FILE *inFile)
{
	while (!feof(inFile))
	{
		while (buffer[0] != '[' && !feof(inFile))
		{
			fgets(buffer,128,inFile);
		}
		fgets(buffer,128,inFile);
		std::string str = std::string(buffer);
		loadLayer(inFile,str,width,height);
	}
	fclose(inFile);
	reloadMapLayer();
	loadObjects();
	makeConnections();
	checkTerrain();
}
void Level::loadObjects()
{
	pickupCount = 0;
	objectLayer.clear();
	objectLayer.resize(height*width);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			unsigned int tmp = origObjectLayer[convertIndex(x, y)];
			if (tmp != 0)
			{
				objectInit(tmp,x,y);
			}
		}
	}
}
void Level::checkTerrain() {
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (origObjectLayer[convertIndex(x, y)] != 0)
			{
				mapLayer[convertIndex(x, y)]->onEnter(objectLayer[convertIndex(x, y)]);
			}
		}
	}
}
void Level::makeConnections()
{
	Trigger *tmp;
	for (unsigned int i = 0; i < MAX_CONNECTIONS; i++)
	{
		std::vector<int> tmpSenders = senders[i];
		for (unsigned int k = 0; k < tmpSenders.size(); k++) {
			//If the connector wasn't placed over a valid trigger,
			//skip it
			Terrain *test = mapLayer.at(tmpSenders[k]);
			if (test->id == m_manager)
			{
				MultipleTerrainManager *manager = (MultipleTerrainManager*) test;
				test = manager->within[1];
			}	
			if (test->isTrigger == false)
				continue;
			tmp = (Trigger*) test;
			for (unsigned int j = 0; j < receivers[i].size(); j++)
			{
				tmp->addConnection(mapLayer.at(receivers[i].at(j)), receivers[i].at(j));
			}
		}
	}
	doActivateQueue();
}
int Level::convertIndex(int x, int y)
{
	return x+y*width;
}
void Level::convertIndex(int input, int &x, int &y)
{
	x = input % width;
	y = input / width;
}
bool Level::assignObject(int x, int y, Object *obj)
{
	bool retVal = false;
	if (objectLayer.size() == 0)
		return false;
	if (objectLayer[convertIndex(x, y)] == NULL)
		retVal = true;
	objectLayer[convertIndex(x,y)] = obj;
	return retVal;
}
bool Level::assignObject(int index, Object *obj)
{
	bool retVal = false;
	if (objectLayer.size() == 0)
		return false;
	if (objectLayer[index] == NULL)
		retVal = true;
	objectLayer[index] = obj;
	return retVal;
}
Object* Level::getObject(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return NULL;
	return objectLayer[convertIndex(x,y)];
}
Object *Level::getObject(int index)
{
	if (index < 0 || index > width*height)
		return NULL;
	return objectLayer[index];
}
Terrain* Level::getTerrain(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return NULL;
	return mapLayer[convertIndex(x, y)];
}
void Level::loadLayer(FILE* inFile, std::string str, int xSize, int ySize)
{
	char buffer[1024] = {0};
	std::string name;
	std::string value;
	size_t split = str.find("=");
	if (split == std::string::npos)
		return;
	name = (str.substr(0,split));
	size_t endStr = std::min(str.rfind('\r'),str.rfind('\n'));
	endStr = std::min(endStr,str.length());
	value = (str.substr(split+1,endStr-(split+1)));
	if (value == "Map")
	{
		fgets(buffer,strlen("data=\r\n"),inFile);
		//On Linux systems, fgets will not read past the first carriage
		//return and end up off by one, so read an extra character here
		fgetc(inFile);
		loadMapLayer(inFile,&origMapLayer,xSize,ySize);
	}
	else if (value == "Objects")
	{
		fgets(buffer,strlen("data=\r\n"),inFile);
		fgetc(inFile);
		loadMapLayer(inFile,&origObjectLayer,xSize,ySize);
	}
	else if (value == "Connections")
	{
		fgets(buffer, strlen("data=\r\n"), inFile);
		fgetc(inFile);
		loadConnections(inFile,xSize,ySize);
	}
}
void Level::loadMapLayer(FILE *inFile,std::vector<unsigned int> *layer,int xSize, int ySize)
{
	int maxChars = 5;
	int maxLine = xSize*maxChars;
	char *buffer = new char[maxLine];
	int val = 0;
	int numRead = 0;
	(*layer).resize(xSize*ySize);
	char *it = buffer;
	for (int y = 0; y < ySize; y++)
	{
		it = buffer;
		if (!fgets(buffer,maxLine,inFile))
		{
			exit(0);
		}
		for (int x = 0; x < xSize; x++)
		{
			if (sscanf(it,"%d,%n",&val,&numRead) == EOF)
			{
				exit(0);
			}
			if (numRead == 0)
			{
				return;
			}
			it += numRead;
			(*layer)[y*ySize+x] = val;
		}
	}
	delete[] buffer;
}
void Level::loadConnections(FILE *inFile, int xSize, int ySize)
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
		receivers[i].clear();
	int maxChars = 5;
	int maxLine = xSize*maxChars;
	char *buffer = new char[maxLine];
	int val = 0;
	int numRead = 0;
	char *it = buffer;
	//Since objects have not been initialied at this point, store just the index
	//of the object to be used, and fill it in later
	for (int y = 0; y < ySize; y++)
	{
		it = buffer;
		if (!fgets(buffer, maxLine, inFile))
		{
			exit(0);
		}
		for (int x = 0; x < xSize; x++)
		{
			if (sscanf(it, "%d,%n", &val, &numRead) == EOF)
			{
				exit(0);
			}
			if (numRead == 0)
			{
				delete buffer;
				return;
			}
			if (val != 0)
			{
				//Subtract the value of this receiver from the lowest possible value:
				//if the modulus is 0, then it is a sender, otherwise it is a receiver.
				//Since there are both senders and receivers for each index, divide the 
				//difference by two in order to get the proper offsets.
				//This is used because it allows for up to the connection limit without
				//needing to modify the code.
				if (val >= m_sender1 && ((val-m_sender1) % 2) == 1)
				{
					receivers[(val - m_sender1) / 2].push_back(convertIndex(x,y));
				}
				else if (val >= m_sender1 && ((val - m_sender1) % 2) == 0)
				{
					senders[(val - m_sender1) / 2].push_back(convertIndex(x, y));
				}
			}
			it += numRead;
		}
	}
	delete[] buffer;
//Some connections should be triggered as soon as the level starts; this does so
}
void Level::reloadMapLayer()
{
	mapLayer.resize(origMapLayer.capacity());
	for (unsigned int i = 0; i < origMapLayer.capacity(); i++)
	{
		mapLayer[i] = NULL;
		applyTerrain(origMapLayer.at(i),i);
	}
}
class Terrain *instantiateTerrain(int input, int i)
{
	Terrain *out = NULL;
	switch (input)
	{
	case m_firefloor:
		out = new FireFloor(i % getCurrentLevel()->width, i / getCurrentLevel()->width);
		break;
	case m_defaultWall:
		out = new Wall(-1);
		break;
	case m_wall:
		out = new Wall(i);
		break;
	case m_conveyorw:
		out = new Conveyor(D_LEFT);
		break;
	case m_conveyors:
		out = new Conveyor(D_DOWN);
		break;
	case m_conveyore:
		out = new Conveyor(D_RIGHT);
		break;
	case m_conveyorn:
		out = new Conveyor(D_UP);
		break;
	case m_exit:
		out = new Exit();
		break;
	case m_pressure:
		out = new PressureSwitch();
		break;
	case m_barrier:
		out = new Barrier();
		break;
	case m_yellowBarrier:
		out = new ColorBarrier(0);
		break;
	case m_icefloor:
		if (getCurrentLevel()->mapLayer.at(i) == NULL) {
			applyTerrain(m_floor, i);
		}
		out = new IceFloor();
		break;
	case m_bomb:
		out = new Bomb();
		break;
	case m_toggleOff:
		out = new ToggleSwitch(false);
		break;
	case m_toggleOn:
		out = new ToggleSwitch(1);
		break;
	case m_dupeN:
	case m_dupeS:
	case m_dupeW:
	case m_dupeE:
		out = new Duplicator(input - m_dupeN + 1);
		break;
	case m_blackHole:
		out = new BlackHole();
		break;
	case m_raisedFloor:
		out = new RaisedFloor();
		break;
	case m_electricFloor:
		out = new ElectricFloor(true);
		break;
	case m_electricFloorOff:
		out = new ElectricFloor(false);
		break;
	case m_freezerFloorOff:
		out = new Freezer(false);
		break;
	case m_freezerFloor:
		out = new Freezer(true);
		break;
	case m_water:
		out = new Water();
		break;
	case m_bounceWall:
		out = new BounceWall();
		break;
	case m_risingWall:
		out = new RisingWall(i);
		break;
	case m_oilspill:
		if (getCurrentLevel()->mapLayer.at(i) == NULL)
			applyTerrain(m_floor, i);
		out = new OilFloor();
		break;
	case m_manager:
		out = new MultipleTerrainManager();
		break;
	case m_teleSource:
		if (getCurrentLevel()->mapLayer.at(i) == NULL)
			applyTerrain(m_floor, i);
		out = new Teleporter();
		break;
	case m_teleDest:
		if (getCurrentLevel()->mapLayer.at(i) == NULL)
			applyTerrain(m_floor, i);
		out = new TeleDestination();
		break;
	case m_floor:
	default:
		out = new Floor();
		break;
	}
	//Give the object the id that was used to create them
	out->id = input;
	//Set the index, but only for objects which don't have it set to -1
	//(meaning the singleton objects)
	out->index = i;
	return out;
}
unsigned char lookupWall(int index)
{
	Level *level = getCurrentLevel();
	int x = index % level->width;
	int y = index / level->width;
	int addBy = 1;
	unsigned char tmp = 0;
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i == x && j == y)
				continue;
			if (i < 0 || i >= level->width || j < 0 || j >= level->height)
			{
				addBy *= 2;
				continue;
			}
			if (level->origMapLayer.at(level->convertIndex(i, j)) == m_wall
				|| i < 0 || i >= level->width || j < 0 || j >= level->height)
				tmp += addBy;
			addBy *= 2;
		}
	return lookupTable[tmp];
}
//Create the global instances to be used for all objects of their type
//Clear the current level and load the next
void switchLevel(int levelNum)
{
	//On most systems we can probably afford to keep the music in memory
	//On the Wii not so much
#ifdef GEKKO
	 freeMusic(currentLevelNum);
#endif
	setTicks();
	dumpMoves();
	currentLevelNum = levelNum;
	std::string tempLevel = constructLevelName(levelNum);
	clearObjects();
	clearTerrain();
	changeText();
	resetDeleteQueue();
	clearLogicList();
	if (loadLevel(tempLevel,levelNum) == 0)
	{
		exit(0);
	}
	displayName = true;
	startLevelName = "";
	doTextBox(player->y);
	changeText();
	won = false;
	lastInputTime = getTicks();
	playMusic(levelNum);
	resetMoveQueue();
	resetCreationQueue();
	resetActivateQueue();
	resetSwitchQueue();
	levelStartTime = getTicks();
	resetMoves();
	getCurrentLevel()->checkTerrain();
	replayEnabled = false;
}
//Make the level name given the number
std::string constructLevelName(int levelNum)
{
	std::string tempLevel = levelPath;
	tempLevel.append(LevelStrings[levelNum]);
	tempLevel.append(".ele");
	return tempLevel;
}
class Level* getCurrentLevel()
{
	return allLevels[currentLevelNum];
}
bool loadLevel(std::string fileName, int levelNum)
{
	currentLevelNum = levelNum;
	char buffer[BUFFERLENGTH];
	playerPlaced = false;
	if (mapLoaded[levelNum] == true)
	{
		Level *tmp = getCurrentLevel();
		tmp->reloadMapLayer();
		tmp->loadObjects();
		tmp->makeConnections();
		changeText();
		return true;
	}
	FILE *ftemp;
	sprintf(buffer, "\nFileName: %s\n", fileName.c_str());
	ftemp = fopen(fileName.c_str(), "rb");
	if (ftemp == NULL)
	{
		exit(0);
	}
	class Level *tmp = new class Level(ftemp, levelNum);
	if (tmp == NULL)
	{
		exit(0);
	}
	mapLoaded[levelNum] = true;
	fclose(ftemp);
	changeText();
	return true;
}
void restartLevel() {
	switchLevel(currentLevelNum);
}
void prevLevel() {
	if (currentLevelNum > 1)
		switchLevel(currentLevelNum - 1);
}
void nextLevel() {
	if (currentLevelNum < MAX_LEVEL-1)
		switchLevel(currentLevelNum + 1);
}
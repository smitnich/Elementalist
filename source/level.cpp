#include "sdlFiles.h"
#include "base.h"
#include "level.h"
#include "objectDef.h"
#include "terrain.h"
#include "tileEnum.h"
#include "wallLookup.h"
#include <queue>
//The number of objects created
extern unsigned int numObjects;
const char* header = "[header]";
const char* layer = "[layer]";
extern bool playerPlaced;
bool mapLoaded[MAX_LEVEL] = { 0 };
//Which level we are on
int levelNum = 1;
//The path to the level
extern string levelPath;
//The name of the starting level
extern string startLevelName;
//The last time to have user input
extern long lastInputTime;
//The last movement of the player
extern int lastMoveDir;
//Whether to display the level name
extern bool displayName;
//The players y position
extern int posY;
//Whether or not the player has won the level
extern bool won;
extern int frame;
extern Mix_Music* levelMusic[MAX_LEVEL];
void freeMusic(int levelNum);
bool loadLevel(string levelName,int levelNum);
void clearObjects();
void doTextBox(int);
void playMusic(int level);
class Terrain *instantiateTerrain(int input, int offset);
extern int currentLevelNum;
void changeText();
long getTicks();
void doActivateQueue();

//For objects that don't have any state, it is pointless
//to create a new instance for each one, so just create
//one instance that all pointers refer to
Terrain *baseFloor = NULL;
Terrain *baseExit = NULL;
Terrain *baseIceFloor = NULL;


Object* objectInit(char id, int x, int y);
string constructLevelName(int);
class Level *allLevels[MAX_LEVEL];
Level::Level(FILE* inFile, int levelNum)
{
	allLevels[levelNum] = this;
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		senders[i] = -1;
	}
	char buffer[128] = {0};
	fgets(buffer,128,inFile);
	if (strncmp(buffer,header,strlen(header)) != 0)
	{
		return;
	}
	string name;
	string value;
	do 
	{
		fgets(buffer,128,inFile);
		string str = string(buffer);
		size_t split = str.find("=");
		if (split == string::npos)
			continue;
		name = (str.substr(0,split));
		value = (str.substr(split+1,string::npos));
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
		string str = string(buffer);
		loadLayer(inFile,str,width,height);
	}
	fclose(inFile);
	reloadMapLayer();
	loadObjects();
	makeConnections();
}
void Level::loadObjects()
{
	Object *tmpObj;
	objectLayer.clear();
	objectLayer.resize(height*width);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			char tmp = origObjectLayer[convertIndex(x, y)];
			if (tmp != 0)
			{
				tmpObj = objectInit(tmp,x,y);
				mapLayer[convertIndex(x, y)]->onEnter(tmpObj);
			}
		}
	}
}
void Level::makeConnections()
{
	Trigger *tmp;
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (senders[i] == -1)
			continue;
		//If the connector wasn't placed over a valid trigger,
		//skip it
		Terrain *test = mapLayer.at(senders[i]);
		if (test->isTrigger == false)
			continue;
		tmp = (Trigger*) mapLayer.at(senders[i]);
		for (unsigned int j = 0; j < receivers[i].size(); j++)
		{
			tmp->connections.insert(tmp->connections.begin(),mapLayer.at(receivers[i].at(j)));
		}
	}
	doActivateQueue();
}
int Level::convertIndex(int x, int y)
{
	return x+y*this->height;
}
bool Level::assignObject(int x, int y, Object *obj)
{
	bool retVal = false;
	if (objectLayer[convertIndex(x,y)] == NULL)
		retVal = true;
	objectLayer[convertIndex(x,y)] = obj;
	return retVal;
}
Object* Level::getObject(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return NULL;
	return objectLayer[convertIndex(x,y)];
}
Terrain* Level::getTerrain(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return NULL;
	return mapLayer[convertIndex(x, y)];
}
void Level::loadLayer(FILE* inFile, string str, int xSize, int ySize)
{
	char buffer[1024] = {0};
	string name;
	string value;
	size_t split = str.find("=");
	if (split == string::npos)
		return;
	name = (str.substr(0,split));
	size_t endStr = min(str.rfind('\r'),str.rfind('\n'));
	endStr = min(endStr,str.length());
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
void Level::loadMapLayer(FILE *inFile,vector<char> *layer,int xSize, int ySize)
{
	int maxChars = 4;
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
	int maxChars = 4;
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
					senders[(val - m_sender1) / 2] = convertIndex(x, y);
				}
			}
			it += numRead;
		}
	}
//Some connections should be triggered as soon as the level starts; this does so
}
void Level::reloadMapLayer()
{
	mapLayer.resize(origMapLayer.capacity());
	for (unsigned int i = 0; i < origMapLayer.capacity(); i++)
	{
		mapLayer[i] = instantiateTerrain(origMapLayer[i], i);
	}
}
class Terrain *instantiateTerrain(int input, int i)
{
	Terrain *out = NULL;
	switch (input)
	{
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
		out = baseExit;
		break;
	case m_pressure:
		out = new PressureSwitch();
		break;
	case m_barrier:
		out = new Barrier();
		break;
	case m_icefloor:
		out = baseIceFloor;
		break;
	case m_bomb:
		out = new Bomb();
		break;
	case m_toggleOff:
		out = new ToggleSwitch(false);
		break;
	case m_toggleOn:
		out = new ToggleSwitch(true);
		break;
	case m_dupeN:
	case m_dupeS:
	case m_dupeW:
	case m_dupeE:
		out = new Duplicator(input - m_dupeN + 1);
		break;
	case m_floor:
	default:
		out = baseFloor;
		break;
	}
	//Set the index, but only for objects which don't have it set to -1
	//(meaning the singleton objects)
	if (out->index != -1)
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
void createGlobalInstances()
{
	baseFloor = new Floor();
	baseExit = new Exit();
	baseIceFloor = new IceFloor();
}
void freeGlobalInstances()
{
	delete baseFloor;
	delete baseExit;
	delete baseIceFloor;
}
//Clear the current level and load the next
void switchLevel(int levelNum)
{
	//On most systems we can probably afford to keep the music in memory
	//On the Wii not so much
#ifndef GEKKO
	 freeMusic(currentLevelNum);
#endif
	currentLevelNum = levelNum;
	string tempLevel = constructLevelName(levelNum);
	clearObjects();
	changeText();
	if (loadLevel(tempLevel,levelNum) == 0)
	{
		exit(0);
	}
	displayName = true;
	startLevelName = "";
	doTextBox(posY);
	changeText();
	won = false;
	lastInputTime = getTicks();
	playMusic(levelNum);
}
//Make the level name given the number
string constructLevelName(int levelNum)
{
	string tempLevel = levelPath;
	tempLevel.append(LevelStrings[levelNum]);
	tempLevel.append(".ele");
	return tempLevel;
}
class Level* getCurrentLevel()
{
	return allLevels[currentLevelNum];
}
bool loadLevel(string fileName, int levelNum)
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
	//ftemp = fopen("levels\\test.txt","r");
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
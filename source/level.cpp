#include "sdlFiles.h"
#include "base.h"
#include "level.h"
#include "objectDef.h"
#include "terrain.h"
#include "pointerContainer.h"
#include <queue>
//The number of objects created
extern unsigned int numObjects;
extern const char* header;
//Which level we are on
int levelNum = 1;
//The path to the level
extern string levelPath;
//The name of the starting level
extern string startLevelName;
//The last frame to have user input
extern int lastInputFrame;
//The last movement of the player
extern int lastMoveDir;
//Whether to display the level name
extern bool displayName;
//The players y position
extern int posY;
//Whether or not the player has won the level
extern bool won;
extern int lastInputFrame;
extern int frame;
extern Mix_Music* levelMusic[MAX_LEVEL];
bool loadLevel(string levelName,int levelNum);
void clearObjects();
void doTextBox(int);
class Terrain *instantiateTerrain(int input);
extern int currentLevelNum;
void changeText();

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
	int numHeaderLines = 4;
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
	loadObjects();
	reloadMapLayer();
	makeConnections();
}
void Level::loadObjects()
{
	objectLayer.resize(height*width);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			char tmp = origObjectLayer[convertIndex(x, y)];
			if (tmp != 0)
			{
				objectInit(tmp,x,y);
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
		for (int j = 0; j < receivers[i].size(); j++)
		{
			tmp->connections.insert(tmp->connections.begin(),mapLayer.at(receivers[i].at(j)));
		}
	}
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
	return objectLayer[convertIndex(x,y)];
}
Terrain* Level::getTerrain(int x, int y)
{
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
				if (val == m_receiver1)
				{
					receivers[0].push_back(convertIndex(x,y));
				}
				else if (val == m_sender1)
				{
					senders[0] = convertIndex(x, y);
				}
			}
			it += numRead;
		}
	}
}
void Level::reloadMapLayer()
{
	mapLayer.resize( origMapLayer.capacity());
	for (unsigned int i = 0; i < origMapLayer.capacity(); i++)
	{
		mapLayer[i] = instantiateTerrain(origMapLayer[i]);
	}
}
class Terrain *instantiateTerrain(int input)
{
	Terrain *out = NULL;
	switch(input)
	{
		case m_wall:
			out = new Wall();
			break;
		case m_floor:
			out = new Floor();
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
		default:
			out = new Floor();
			break;
	}
	return out;
}
//Clear the current level and load the next
void switchLevel(int levelNum)
{
	currentLevelNum = levelNum;
	string tempLevel = constructLevelName(levelNum);
	clearObjects();
	changeText();
	if (loadLevel(tempLevel,levelNum) == 0)
	{
		exit(0);
	}
	displayName = 1;
	startLevelName = "";
	doTextBox(posY);
	changeText();
	won = false;
	lastInputFrame = frame;
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
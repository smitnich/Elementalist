#include "base.h"
#include "map.h"
#include "level.h"
const char* header = "[header]"; 
const char* layer = "[layer]";
char **rawObjectLayer;
Level *getCurrentLevel();
//Load the level by initializing static features, filling out the varMap and creating objects
bool loadLevel(string fileName, int levelNum)
{
	bool loadFromFile = true;
	char buffer[BUFFERLENGTH];
	char c;
	int i;
	playerPlaced = 0;
	if (mapLoaded[levelNum] == 1)
	{
		Level *tmp = getCurrentLevel();
		tmp->reloadMapLayer();
		tmp->loadObjects();
		changeText();
		return true;
	}
	totalPressureCount = 0;
	int x = 0;
	int y = 0;
	FILE *ftemp;
	sprintf(buffer,"\nFileName: %s\n",fileName.c_str());
	ftemp = fopen(fileName.c_str(),"rb");
	//ftemp = fopen("levels\\test.txt","r");
	if (ftemp == NULL)
	{
		exit(0);
	}
	class Level *tmp = new class Level(ftemp,levelNum);
	if (tmp == NULL)
	{
		exit(0);
	}
	for (x = 0; x < tmp->width; x++)
	{
		for (y = 0; y < tmp->width; y++)
		{
			map[x][y] = tmp->origMapLayer[tmp->convertIndex(x, y)];
		}
	}
	mapLoaded[levelNum] = 1;
	fclose(ftemp);
	changeText();
	return true;
}
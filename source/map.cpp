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
//Check if the given values correspond to this particular location
//Used to determine which wall sprite to use
bool checkArray(int v_nw, int v_n, int v_ne, int v_w, int v_e,
	int v_sw, int v_s, int v_se, int x, int y)
	{
	bool pickthis = 1;
	int newMap[3][3];
	newMap[0][0] = map[x-1][y-1];
	newMap[1][0] = map[x][y-1];
	newMap[2][0] = map[x+1][y-1];
	newMap[0][1] = map[x-1][y];
	newMap[1][1] = map[x][y];
	newMap[2][1] = map[x+1][y];
	newMap[0][2] = map[x-1][y+1];
	newMap[1][2] = map[x][y+1];
	newMap[2][2] = map[x+1][y+1];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (newMap[i][j] != m_wall)
				newMap[i][j] = m_floor;
		}
		if (v_nw != m_any && v_nw != newMap[0][0])
			pickthis = 0;
		else if (v_n != m_any && v_n != newMap[1][0])
			pickthis = 0;
		else if (v_ne != m_any && v_ne != newMap[2][0])
			pickthis = 0;
		else if (v_w != m_any && v_w != newMap[0][1])
			pickthis = 0;
		else if (v_e != m_any && v_e != newMap[2][1])
			pickthis = 0;
		else if (v_sw != m_any && v_sw != newMap[0][2])
			pickthis = 0;
		else if (v_s != m_any && v_s != newMap[1][2])
			pickthis = 0;
		else if (v_se != m_any && v_se != newMap[2][2])
			pickthis = 0;
		return pickthis;
}
//Choose which wall based on its surroundings as well as whether it is next to any edges
int pickWall(int x, int y)
{
	if (y == 0)
	{
		if (x == 0)
		{
			if (map[x+1][y+1] == m_wall)
				return v_wall1;
			else
				return v_wall15;
		}
		else if (x == MAP_SIZE-1)
		{
			if (map[x-1][y+1] == m_wall)
				return v_wall1;
			else
				return v_wall12;
		}
		else
		{
			if (map[x][y+1] == m_wall &&  map[x-1][y+1] != m_wall && map[x+1][y+1] !=m_wall)
				return v_wall9;
			else if (map[x][y+1] == m_wall && map[x-1][y+1] == m_wall && map[x+1][y+1] != m_wall)
				return v_wall12;
			else if (map[x][y+1] == m_wall && map[x-1][y+1] != m_wall && map[x+1][y+1] == m_wall)
				return v_wall15;
			else if (map[x][y+1] == m_wall && map[x-1][y+1] == m_wall && map[x+1][y+1] == m_wall)
				return v_wall1;
			else
				return v_wallewn1;
		}
	}
	else if (y == MAP_SIZE-1)
	{
		if (x == 0)
		{
			if (map[x+1][y-1] == m_wall)
				return v_wall1;
			else
				return v_wall3;
		}
		else if (x == MAP_SIZE-1)
		{
			if (map[x-1][y-1] == m_wall)
				return v_wall1;
			else
				return v_wall2;
		}
		else
		{
			if (map[x][y-1] == m_wall && map[x-1][y-1] != m_wall && map[x+1][y-1] == m_wall)
				return v_wall2;
			else if (map[x][y-1] == m_wall && map[x-1][y-1] == m_wall && map[x+1][y-1] != m_wall)
				return v_wall3;
			else if (map[x][y-1] == m_wall && map[x-1][y-1] != m_wall && map[x+1][y-1] != m_wall)
				return v_wall4;
			else if (map[x][y-1] == m_wall)
				return v_wall1;
			else
				return v_wallsew1;
		}
	}
	else
	{
		if (x == 0)
		{
			if (map[x+1][y] == m_wall && map[x+1][y-1] != m_wall && map[x+1][y+1] == m_wall)
				return v_wall3;
			else if (map[x+1][y] ==m_wall && map[x+1][y-1] == m_wall && map[x+1][y+1] != m_wall)
				return v_wall12;//originally 15
			else if (map[x+1][y] == m_wall && map[x+1][y-1]!= m_wall&& map[x+1][y+1] != m_wall)
				return v_wall16;
			else if (map[x+1][y] == m_wall)
				return v_wall1;
			else
				return v_wallwsn1;
		}
		else if (x == MAP_SIZE-1)
		{
			if (map[x-1][y] == m_wall && map[x-1][y-1] != m_wall && map[x-1][y+1] != m_wall)
				return v_wall15;//originally 12
			else if (map[x-1][y] == m_wall && map[x-1][y-1] != m_wall && map[x-1][y+1] == m_wall)
				return v_wall2;
			else if (map[x-1][y] == m_wall && map[x-1][y-1] == m_wall && map[x-1][y+1] == m_wall)
				return v_wall1;
			else if (map[x-1][y] == m_wall && map[x-1][y-1] != m_wall && map[x-1][y+1] == m_wall)
				return v_wall13;
			else
				return v_wallesn1;
		}
		else
		{
			//Todo: Try to fix this crap
			if (checkArray(m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,x,y))
				return v_wall1;
			else if (checkArray(m_floor,m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,x,y))
				return v_wall2;
			else if (checkArray(m_wall,m_wall,m_floor,m_wall,m_wall,m_wall,m_wall,m_wall,x,y))
				return v_wall3;
			else if (checkArray(m_floor,m_wall,m_floor,m_wall,m_wall,m_wall,m_wall,m_wall,x,y))
				return v_wall4;
			else if (checkArray(m_floor,m_wall,m_floor,m_wall,m_wall,m_floor,m_wall,m_wall,x,y))
				return v_wall5;
			else if (checkArray(m_floor,m_wall,m_floor,m_wall,m_wall,m_floor,m_wall,m_floor,x,y))
				return v_wall6;
			else if (checkArray(m_floor,m_wall,m_floor,m_wall,m_wall,m_wall,m_wall,m_floor,x,y))
				return v_wall7;
			else if (checkArray(m_floor,m_wall,m_wall,m_wall,m_wall,m_floor,m_wall,m_floor,x,y))
				return v_wall8;
			else if (checkArray(m_wall,m_wall,m_wall,m_wall,m_wall,m_floor,m_wall,m_floor,x,y))
				return v_wall9;
			else if (checkArray(m_wall,m_wall,m_floor,m_wall,m_wall,m_floor,m_wall,m_floor,x,y))
				return v_wall10;
			else if (checkArray(m_wall,m_wall,m_floor,m_wall,m_wall,m_floor,m_wall,m_wall,x,y))
				return v_wall11;
			else if (checkArray(m_wall,m_wall,m_wall,m_wall,m_wall,m_floor,m_wall,m_wall,x,y))
				return v_wall12;
			else if (checkArray(m_floor,m_wall,m_wall,m_wall,m_wall,m_floor,m_wall,m_wall,x,y))
				return v_wall13;
			else if (checkArray(m_floor,m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,m_floor,x,y))
				return v_wall14;
			else if (checkArray(m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,m_wall,m_floor,x,y))
				return v_wall15;
			else if (checkArray(m_wall,m_wall,m_floor,m_wall,m_wall,m_wall,m_wall,m_floor,x,y))
				return v_wall16;
			else if (checkArray(m_any,m_floor,m_any,m_floor,m_floor,m_any,m_floor,m_any,x,y))
				return v_wallbase;
			else if (checkArray(m_any,m_floor,m_any,m_floor,m_wall,m_any,m_floor,m_any,x,y))
				return v_walle;
			else if (checkArray(m_any,m_wall,m_wall,m_floor,m_wall,m_any,m_wall,m_wall,x,y))
				return v_wallesn1;
			else if (checkArray(m_any,m_wall,m_floor,m_floor,m_wall,m_any,m_wall,m_wall,x,y))
				return v_wallesn2;
			else if (checkArray(m_any,m_wall,m_floor,m_floor,m_wall,m_any,m_wall,m_floor,x,y))
				return v_wallesn3;
			else if (checkArray(m_any,m_wall,m_wall,m_floor,m_wall,m_any,m_wall,m_floor,x,y))
				return v_wallesn4;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallew;
			else if (checkArray(m_wall,m_wall,m_wall,m_wall,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallewn1;
			else if (checkArray(m_wall,m_wall,m_floor,m_wall,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallewn2;
			else if (checkArray(m_floor,m_wall,m_wall,m_wall,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallewn3;
			else if (checkArray(m_floor,m_wall,m_floor,m_wall,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallewn4;
			else if (checkArray(m_any,m_wall,m_any,m_floor,m_floor,m_any,m_floor,m_any,x,y))
				return v_walln;
			else if (checkArray(m_any,m_wall,m_any,m_floor,m_floor,m_any,m_wall,m_any,x,y))
				return v_wallns;
			else if (checkArray(m_any,m_floor,m_any,m_floor,m_floor,m_any,m_wall,m_any,x,y))
				return v_walls;
			else if (checkArray(m_any,m_floor,m_any,m_floor,m_wall,m_any,m_wall,m_wall,x,y))
				return v_wallse1;
			else if (checkArray(m_any,m_floor,m_any,m_floor,m_wall,m_any,m_wall,m_floor,x,y))
				return v_wallse2;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_wall,m_wall,m_wall,m_wall,x,y))
				return v_wallsew1;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_wall,m_floor,m_wall,m_wall,x,y))
				return v_wallsew2;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_wall,m_floor,m_wall,m_floor,x,y))
				return v_wallsew3;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_wall,m_wall,m_wall,m_floor,x,y))
				return v_wallsew4;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_floor,m_wall,m_wall,m_any,x,y))
				return v_wallsw1;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_floor,m_floor,m_wall,m_any,x,y))
				return v_wallsw2;
			else if (checkArray(m_any,m_floor,m_any,m_wall,m_floor,m_any,m_floor,m_any,x,y))
				return v_wallw;
			else if (checkArray(m_wall,m_wall,m_any,m_wall,m_floor,m_wall,m_wall,m_any,x,y))
				return v_wallwsn1;
			else if (checkArray(m_floor,m_wall,m_any,m_wall,m_floor,m_wall,m_wall,m_any,x,y))
				return v_wallwsn2;
			else if (checkArray(m_floor,m_wall,m_any,m_wall,m_floor,m_floor,m_wall,m_any,x,y))
				return v_wallwsn3;
			else if (checkArray(m_wall,m_wall,m_any,m_wall,m_floor,m_floor,m_wall,m_any,x,y))
				return v_wallwsn4;
			else if (checkArray(m_any,m_wall,m_wall,m_floor,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallne1;
			else if (checkArray(m_any,m_wall,m_floor,m_floor,m_wall,m_any,m_floor,m_any,x,y))
				return v_wallne2;
			else if (checkArray(m_wall,m_wall,m_any,m_wall,m_floor,m_any,m_floor,m_any,x,y))
				return v_wallnw1;
			else if (checkArray(m_floor,m_wall,m_any,m_wall,m_floor,m_any,m_floor,m_any,x,y))
				return v_wallnw2;
		}
	}
	return -1;
}

#ifndef _LEVEL
#define _LEVEL
#include "terrain.h"
//The levelnames of the basic "campaign"
const string LevelStrings[MAX_LEVEL] = 
{
"Dummy",//0
"test",//1
"test2",//2
"test3",//3
"test4"//4
};
class Level
{
	public:
	int width;
	int height;
	vector<char> origMapLayer;
	vector<char> origObjectLayer;
	vector<Terrain *> mapLayer;
	vector<Object *> objectLayer;
	vector<int> receivers[MAX_CONNECTIONS];
	int senders[MAX_CONNECTIONS];
	char **connectionLayer;
	Level(FILE* inFile, int levelNum);
	void loadAllLayers(char *buffer,FILE *inFile);
	void loadObjects();
	int convertIndex(int x, int y);
	void loadLayer(FILE *inFile, string str, int xSize, int ySize);
	void loadMapLayer(FILE *inFile,vector<char> *layer,int xSize, int ySize);
	void reloadMapLayer();
	bool assignObject(int x, int y, Object *obj);
	Object* getObject(int x, int y);
	Terrain* getTerrain(int x, int y);
	void loadConnections(FILE *inFile, int xSize, int ySize);
	void makeConnections();
};
#endif
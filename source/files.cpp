#include "files.h"
//Creates a simple, empty file to be edited by the user
//Todo: Modify to use the level directory
#ifdef LINUX
#define OPERATINGSYSTEM "LINUX"
#elif _WIN32
#define OPERATINGSYSTEM "WINDOWS"
#elif GEKKO
#define OPERATINGSYSTEM "WII"
#endif
extern string directorySymbol;
void mkTemplate()
{
	int x = 0;
	int y = 0;
	FILE *ftemp;
	ftemp = fopen("/apps/Elementalist/levels/template.ele","wb");
	rewind(ftemp);
	for (y = 0; y < MAP_SIZE; y++)
	{
		for (x = 0; x < MAP_SIZE+1; x++)
		{
			if (x == 0||x==MAP_SIZE-1||y==0||y==MAP_SIZE-1)
				fputs("#",ftemp);
			else if (x == MAP_SIZE/2 && y == MAP_SIZE/2)
				fputs("@",ftemp);
			else
				fputs(".",ftemp);
		}
		fputs("\n",ftemp);
	}
	fclose(ftemp);
}
//Saves the level as it currently appears
//Todo: Finish this
//Implement a level editor
void saveLevel(char map[MAP_SIZE][MAP_SIZE], int posX, int posY, string fileName)
{
	fileName.append(".ele");
	int x = 0;
	int y = 0;
	FILE *ftemp;
	ftemp = fopen(fileName.c_str(),"wb");
	rewind(ftemp);
	for (y = 0; y < MAP_SIZE; y++)
	{
		for (x = 0; x < MAP_SIZE; x++)
		{
			if (x == posX && y == posY)
				fputs("@",ftemp);
			else if (map[x][y] == m_floor)
				fputs(".",ftemp);
			else if (map[x][y] == m_wall)
				fputs("#",ftemp);
			else
				fputs(" ",ftemp);
		}
		fputs("\n",ftemp);
	}
	fclose(ftemp);
}
//Gets the length of a file passed in
int getFileLength(char fileName[])
{
	struct stat st;
	stat(fileName, &st);
	return st.st_size;
}
//Runs all lines through parseconfig
void loadConfig()
{
	int counter = 0;
	char c;
	char buffer[BUFFERLENGTH];
	FILE *config;
	config = fopen("config.txt","rb");
	if (config == NULL)
		return;
	int fileLength = getFileLength(const_cast <char *> ("config.txt"));
	int i = 0;
	int length = 0;
	string bufferString;
	while (counter < fileLength)
	{
		c = fgetc(config);
		counter++;
		//Check for endings: 10 = \n 13 = \r
		if (c == 10 || c == 13 || c == 'ÿ' || c == ';')
		{
			length = i;
			i = 0;
			bufferString.append(buffer,length);
			if (bufferString.empty() == 0)
				parseConfig(const_cast<char *> (bufferString.data()));
			bufferString.clear();
		}
		else
		{
			buffer[i] = c;
			i++;
		}
	}
}
void fileInit()
{
	//Wii only, initialize all possible storage devices
#ifdef GEKKO
	fatMountSimple("sd", &__io_wiisd);
	fatMountSimple("usb", &__io_usbstorage);
	/*Only try to mount these if requested: little reason to use*/
	if (markCard[0] == true)
	fatMountSimple("carda",&__io_gcsda);
	if (markCard[1] == true)
	fatMountSimple("cardb",&__io_gcsdb);
	ntfsCount = ntfsMountAll(&mounts, NTFS_DEFAULT | NTFS_RECOVER);
	chdir(appPath.data());
#endif
	//Open the output file, print which operating system is in use
	outputFile = fopen("output.txt","wb");
	outputLog("Running on ");
	outputLog(OPERATINGSYSTEM);
	outputLog("\n\r");
	outputLog(const_cast <char *> (appPath.data()));
	loadConfig();
}

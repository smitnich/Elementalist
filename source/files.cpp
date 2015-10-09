#include "files.h"
//Creates a simple, empty file to be edited by the user
//Todo: Modify to use the level directory
#ifdef LINUX
#define OPERATINGSYSTEM "LINUX"
#elif _WIN32
#define OPERATINGSYSTEM "WINDOWS"
#elif GEKKO
extern std::string appPath;
#define OPERATINGSYSTEM "WII"
#endif
#include <stdio.h>
#ifndef GEKKO
#include <sys/stat.h>
#endif
#include "defs.h"
extern char directorySymbol;
bool markCard[2] = { false, false };

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
	std::string bufferString;
	while (counter <= fileLength)
	{
		c = fgetc(config);
		counter++;
		//Check for endings: 10 = \n 13 = \r
		if (c == 10 || c == 13 || c == 'ÿ' || c == ';' || counter > fileLength)
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
	//ntfsCount = ntfsMountAll(&mounts, NTFS_DEFAULT | NTFS_RECOVER);
	chdir(appPath.data());
#endif
	//Open the output file, print which operating system is in use
	loadConfig();
}

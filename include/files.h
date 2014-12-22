//#include <ext2.h>
#include "base.h"
//If wii
#ifdef GEKKO
#include <sdcard/gcsd.h>
//The ntfs mounts pointer
ntfs_md *mounts = NULL;
//The number of ntfs drives mounted
int ntfsCount = 0;
#endif
extern string appPath;
void parseConfig(char[]);
void outputLog(const char[]);
void outputLog(char[]);
void outputLog(int);
//Where the output is written
FILE *outputFile = NULL;
bool markCard[2] = {false,false};
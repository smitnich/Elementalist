//#include <ext2.h>
#include <iostream>
//If wii
#ifdef GEKKO
#include "wii.h"
#include <sdcard/gcsd.h>
//The ntfs mounts pointer
ntfs_md *mounts = NULL;
//The number of ntfs drives mounted
int ntfsCount = 0;
#endif
void parseConfig(char[]);
//Where the output is written
bool markCard[2] = {false,false};
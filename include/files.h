#include <iostream>
//If wii
#ifdef GEKKO
#include "wii.h"
#include <sdcard/gcsd.h>
#endif
void parseConfig(char[]);
void loadConfig();
void fileInit();
//Where the output is written
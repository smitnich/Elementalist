#ifdef GEKKO
#include <fat.h>
#include <sdcard/wiisd_io.h>
#include <sys/dir.h>
#include <gccore.h>
#include <wiiuse/wpad.h> 
#include <ogc/wiilaunch.h>
#include <ogcsys.h>
#include <unistd.h>
#include <ntfs.h>
int wupcInput(int wpadnum);
int GCInput(u16 GCButtonsDown);
bool checkGC();
#endif
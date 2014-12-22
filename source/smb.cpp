#ifdef GEKKO
#include "base.h"
#include <smb.h>
#include <stdio.h>
#include <network.h>
string smbUserName;
string smbPassword;
string smbIp;
string smbFolderName;
extern string appPath;
extern string levelPath;
void outputLog(char *);
void outputLog(const char*);
void outputLog(int);
//Attempt to connect tot the given SMB share
//Requires Username, password, IP address, and folder name
bool smbInit()
{
	if (appPath.find("smb:") == string::npos && levelPath.find("smb:") == string::npos)
		return 0;
	net_init();
	if (smbUserName.empty())
		outputLog("User name is not entered\n");
	if (smbPassword.empty())
		outputLog("Password is not entered\n");
	if (smbIp.empty())
		outputLog("IP address not entered");
	if (smbFolderName.empty())
		outputLog("Folder name not entered");
	return smbInitDevice("smb", smbUserName.data(), smbPassword.data(), smbFolderName.data(), smbIp.data());
}
#endif
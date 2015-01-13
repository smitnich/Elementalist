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
//Attempt to connect tot the given SMB share
//Requires Username, password, IP address, and folder name
bool smbInit()
{
	if (appPath.find("smb:") == string::npos && levelPath.find("smb:") == string::npos)
		return 0;
	net_init();
	return smbInitDevice("smb", smbUserName.data(), smbPassword.data(), smbFolderName.data(), smbIp.data());
}
#endif
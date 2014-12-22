#include "config.h"
//Changes values that are either loaded from config.txt or through arguments
void parseConfig(char config[])
{
	string strConfig(config);
	//Assign a level loaded through the executable
	if (strConfig.rfind(".ele") != string::npos)
	{
		loadLevelFromArg(strConfig);
		return;
	}
	//Ignore lines that start with this character
	if (strConfig.at(0) == '#')
	{
		return;
	}
	//Find where the split between key and value is: either ':' or '=' characters
	size_t split = strConfig.find(":");
	//If the ':' is not found
	if (split == string::npos)
	{
		split = strConfig.find("=");
		if (split == string::npos)
		{
			//If neither character is found, abort the function
			return;
		}
	}
	//Split the string into two strings using the split value
	string name (strConfig.substr(0,split));
	string value (strConfig.substr(split+1,string::npos));
	string::size_type i = 0;
	//Remove all newlines from the string to be processed
	while (i < value.length()) {
		i = value.find('\n', i);
		if (i == string::npos) {
			break;
		}
		value.erase(i);
		value.erase(i+1);
	}
	i = 0;
	//Remove line feeds as well
	while (i < value.length()) {
		i = value.find('\r', i);
		if (i == string::npos) {
			break;
		}
		value.erase(i);
		value.erase(i+1);
	}
	//Change the basic file location: all others stem from this unless otherwise modified
	if (name == "basePath")
	{
		appPath.assign(value);
		defaultPath = 0;
		return;
	}
	//Align the interface to the left or the right
	else if (name == "align")
	{
		if (value == "left")
		{
			xInitial = 192;
			alignLeft = 1;
		}
		else if (value == "right")
		{
			xInitial = 24;
			alignLeft = 0;
		}
		return;
	}
	//Change the levelpath
	else if (name == "levelPath")
	{
		levelPath.assign(value);
		if (levelPath.find(":") == string::npos)
		{
			string temp (appPath);
			temp.append(levelPath);
			levelPath.assign(temp);
		}
		return;
	}
	//Check for valid bitDepths and assign
	else if (name == "bitDepth")
	{

		if (value == "8")
			bitDepth = 8;
		else if(value == "16")
			bitDepth = 16;
		else if (value == "24")
			bitDepth = 24;
		else if (value == "32")
			bitDepth = 32;
	}
	else if (name == "musicPath")
	{
		musicPath.assign(value);
		outputLog("Music path changed");
	}
	//Wii only
#ifdef GEKKO
	//Check for various Samba information for loading files over the network
	else if (name == "smbUserName")
	{
		smbUserName.assign(value);
	}
	else if (name == "smbPassword")
	{
		smbPassword.assign(value);
	}
	else if (name == "smbIp")
	{
		smbIp.assign(value);
	}
	else if (name == "smbFolder")
	{
		smbFolderName.assign(value);
	}
#else
	//Not Wii
	//Change whether to run in fullScreen
	else if (name == "fullScreen")
	{
		if (value == "true" || value == "1")
			fullScreen = true;
		else if (value == "false" ||  value == "0")
			fullScreen = false;
	}
	//The horizontal resolution
	else if (name == "resolutionX" || name == "resolutionx")
	{
		videoSizeX = atoi(value.c_str());
	}
	//The vertical resolution
	else if (name == "resolutionY" || name == "resolutiony")
	{
		videoSizeY = atoi(value.c_str());
	}
	//The refresh rate of the game
	else if (name == "FPS")
	{
		framesPerSecond = atoi(value.c_str());
		fpsModifier = (double) 60/framesPerSecond;
		movespeed = (double) 3*fpsModifier;
	}
	else if (name == "NUM_TILES")
	{
		tilesX = atoi(value.c_str());
		tilesY = tilesX;
	}
#endif
}
/*Modify the levelname and */
void loadLevelFromArg(string strConfig)
{
		startLevel.assign(strConfig);
		size_t first = strConfig.rfind(directorySymbol);
		size_t last = strConfig.rfind(".ele");
		last -= first;
		startLevelName.assign(strConfig.substr(first+1,last-1));
		currentLevelNum = 0;
}
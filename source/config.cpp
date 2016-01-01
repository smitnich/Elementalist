#include "config.h"

//Changes values that are either loaded from config.txt or through arguments
void parseConfig(char config[])
{
	std::string strConfig(config);
	//Assign a level loaded through the executable
	if (strConfig.rfind(".ele") != std::string::npos)
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
	if (split == std::string::npos)
	{
		split = strConfig.find("=");
		if (split == std::string::npos)
		{
			//If neither character is found, abort the function
			return;
		}
	}
	//Split the string into two strings using the split value
	std::string name(strConfig.substr(0, split));
	std::string value(strConfig.substr(split + 1, std::string::npos));
	std::string::size_type i = 0;
	//Remove all newlines from the string to be processed
	while (i < value.length()) {
		i = value.find('\n', i);
		if (i == std::string::npos) {
			break;
		}
		value.erase(i);
		value.erase(i+1);
	}
	i = 0;
	//Remove line feeds as well
	while (i < value.length()) {
		i = value.find('\r', i);
		if (i == std::string::npos) {
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
	//Change the levelpath
	else if (name == "levelPath")
	{
		levelPath.assign(value);
		if (levelPath.find(":") == std::string::npos)
		{
			std::string temp(appPath);
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
	}
#ifndef GEKKO
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
#endif
}
void loadLevelFromArg(std::string strConfig)
{
		startLevel.assign(strConfig);
		size_t first = strConfig.rfind(directorySymbol);
		size_t last = strConfig.rfind(".ele");
		last -= first;
		startLevelName.assign(strConfig.substr(first+1,last-1));
		currentLevelNum = 0;
}
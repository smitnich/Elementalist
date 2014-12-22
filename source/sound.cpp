#include "base.h"
#include "sdlFiles.h"
Mix_Chunk *pushBlock;
void outputLog(const char[]);
void outputLog(char[]);
void outputLog(int);
bool channelArray[MAX_CHANNELS];
bool levelMusicLoaded[MAX_LEVEL];
Mix_Music *levelMusic[MAX_LEVEL];
extern string appPath;
string musicPath;
Mix_Music* loadMusic(char *fileName);
/*SDL WII bug avoidance*/
void setPanning(unsigned int channel, unsigned int right)
{
	//Fix the reversed panning on the wii
#ifdef GEKKO
	Mix_SetPanning(channel, right, 254-right);
#else
	Mix_SetPanning(channel,254-right,right);
#endif
}
//Initializes the music
void musicInit()
{
	int i;
	for (i = 0; i < MAX_LEVEL; i++)
	{
		levelMusicLoaded[i] = 0;
		levelMusic[i] = NULL;
	}
	musicPath.assign("sound/");
	if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		outputLog("Unable to open audio");
		outputLog(Mix_AllocateChannels(-1));
	}
}
Mix_Music* loadMusic(char *fileName)
{
	Mix_Music* music;
	char buffer[80];
	outputLog("Music path is: ");
	outputLog(musicPath.c_str());
	sprintf(buffer,"%c%c",musicPath.c_str(),fileName);
	music = Mix_LoadMUS(fileName);
	if (music == NULL)
	{
		outputLog("Failed to load ");
		outputLog(buffer);
	}
	return music;
}
//Todo: Add Sound
bool playSound(Mix_Chunk *input)
{
	if (Mix_PlayChannel( 0, input, 0 ) == -1)
		return 0;
	else
	{
		return 1;
	}
}

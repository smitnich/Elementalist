#include "sdlFiles.h"
#include <iostream>
#include "defs.h"
#include "sound.h"
Mix_Chunk *pushBlock;
bool channelArray[MAX_CHANNELS];
bool levelMusicLoaded[MAX_LEVEL];
char *musicNames[] = { "Dystopic-Factory.mp3", "Puzzle-Game-5.mp3" };
int numSongs = sizeof(musicNames) / sizeof(musicNames[0]);
Mix_Music *levelMusic[sizeof(musicNames)/sizeof(musicNames[0])];
extern std::string appPath;
std::string musicPath;
Mix_Chunk *snd_explode = NULL;
Mix_Chunk *snd_switch = NULL;
Mix_Chunk *snd_teleport = NULL;
bool audioEnabled = true;
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
	numSongs = sizeof(musicNames) / sizeof(musicNames[0]);
	int i;
	for (i = 0; i < MAX_LEVEL; i++)
	{
		levelMusicLoaded[i] = 0;
		levelMusic[i] = NULL;
	}
	musicPath.assign("music/");
	if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		audioEnabled = false;
		return;
	}
	Mix_Volume(-1, 0);
	snd_explode = Mix_LoadWAV("sound/explode.wav");
	snd_switch = Mix_LoadWAV("sound/switch.wav");
	snd_teleport = Mix_LoadWAV("sound/teleport.wav");

}
void freeMusic(int levelNum)
{
	if (!audioEnabled)
		return;
	if (levelNum < 0 || levelNum > MAX_LEVEL || levelMusic[levelNum-1] == NULL)
		return;
	Mix_HaltMusic();
	Mix_FreeMusic(levelMusic[levelNum - 1]);
	levelMusic[levelNum - 1] = NULL;
}
void playMusic(int levelNum)
{
	Mix_Volume(-1, 128);
	if (!audioEnabled)
		return;
	if (levelNum < 0 || levelNum > MAX_LEVEL)
		return;
	char *fileName = musicNames[(levelNum-1) % numSongs];
	char buffer[80];
	if (levelMusic[(levelNum - 1) % numSongs] == NULL)
	{
		sprintf(buffer, "%s%s", musicPath.c_str(), fileName);
		levelMusic[(levelNum - 1) % numSongs] = Mix_LoadMUS(buffer);
	}
	Mix_PlayMusic(levelMusic[(levelNum-1) % numSongs], -1);
}
bool playSound(Mix_Chunk *input)
{
	Mix_Volume(-1, 128);
	if (!audioEnabled)
		return false;
	if (input == NULL)
		return false;
	if (Mix_PlayChannel( 0, input, 0 ) == -1)
		return false;
	else
		return true;
}

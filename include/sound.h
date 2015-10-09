struct Mix_Chunk;
extern Mix_Chunk* snd_explode;
bool playSound(Mix_Chunk *input);
void musicInit();
void setPanning(unsigned int channel, unsigned int right);
Mix_Music* loadMusic(char *fileName);
void freeMusic(int levelNum);
void playMusic(int levelNum);
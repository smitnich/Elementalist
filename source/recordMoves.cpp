unsigned long getTicks();
int determineInput(bool mouse);
#include <list>
#include "inputDef.h"
#include <stdio.h>
#include <stdlib.h>

void writeDebugText(char *in);
const char *replayPath = "replays";
extern char directorySymbol;

extern int currentLevelNum;
int lastRecordedMove;
extern unsigned long levelStartTime;
int currentReplayInput;

#define ARRAY_SIZE 1024*10

struct input_t {
	int button;
	unsigned long time;
};
std::list<input_t> inputArray;

void recordMove() {
	int button = determineInput(false);
	if (button == lastRecordedMove) {
		lastRecordedMove = button;
		return;
	}
	lastRecordedMove = button;
	unsigned long time = getTicks() - levelStartTime;
	input_t input;
	input.button = (char)button;
	input.time = time;
	inputArray.push_back(input);
}
void resetMoves() {
	inputArray.resize(ARRAY_SIZE);
	inputArray.clear();
	currentReplayInput = INPUT_NONE;
}
void dumpMoves() {
	if (inputArray.size() == 0) {
		return;
	}
	char buffer[128];
	sprintf(buffer, "%s%c%d.rep", replayPath, directorySymbol, currentLevelNum);
	FILE *output = fopen(buffer,"r");
	if (output != NULL) {
		fclose(output);
		return;
	}
	output = fopen(buffer, "w");
	int initTime = inputArray.begin()->time;
	for (std::list<input_t>::iterator it = inputArray.begin(); it != inputArray.end(); ++it) {
		sprintf(buffer,"%d,%lu\n", (int) it->button,it->time-initTime);
		fputs(buffer,output);
	}
	fclose(output);
}
bool loadMoves() {
	char buffer[128];
	resetMoves();
	sprintf(buffer, "%s%c%d.rep", replayPath, directorySymbol, currentLevelNum);
	FILE *input = fopen(buffer, "r");
	if (input == NULL) {
		return false;
	}
	while (!feof(input)) {
		input_t inputButton = { 0, 0 };
		if (fscanf(input, "%d,%lu\n", &inputButton.button, &inputButton.time) != 2) {
			return false;
		}
		inputArray.push_back(inputButton);
	}
	fclose(input);
	return true;
}
int getNextReplayMove() {
	if (inputArray.size() == 0) {
		return INPUT_NONE;
	}
	input_t nextInput = inputArray.front();
	char buffer[128];
	sprintf(buffer, "Next Move: %d at %lu", nextInput.button, nextInput.time);
	writeDebugText(buffer);
	if (getTicks() - levelStartTime >= nextInput.time) {
		inputArray.pop_front();
		currentReplayInput = nextInput.button;
		return currentReplayInput;
	}
	return currentReplayInput;
}

unsigned long getTicks();
int determineInput(bool mouse);
#include <vector>
#include "inputDef.h"
#include <stdio.h>
#include <stdlib.h>
#include "recordMoves.h"

void writeDebugText(char *in);
const char *replayPath = "replays";
extern char directorySymbol;

extern int currentLevelNum;
unsigned long lastRecordTime = 0;
extern unsigned long levelStartTime;
int currentReplayInput;

unsigned long replayStartTime = 0;
#define ARRAY_SIZE 1024*10

struct input_t {
	int button;
	unsigned long time;
};
std::vector<input_t> inputArray;

void recordMove() {
	int button = determineInput(false);
	unsigned long time = getTicks() - levelStartTime;
	//Don't record the same move multiple times if there are multiple
	//player objects in play
	if (time == lastRecordTime) {
		return;
	}
	lastRecordTime = time;
	input_t input;
	input.button = (char)button;
	input.time = time;
	inputArray.push_back(input);
}
void unrecordMove()
{
	if (inputArray.size() > 0)
		inputArray.pop_back();
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
	for (std::vector<input_t>::iterator it = inputArray.begin(); it != inputArray.end(); ++it) {
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
	replayStartTime = getTicks();
	return true;
}
int getNextReplayMove(int i) {
	if (inputArray.size() == 0) {
		return INPUT_NONE;
	}
	input_t nextInput = inputArray.at(i);
	char buffer[128];
	sprintf(buffer, "Next Move: %d at %lu", nextInput.button, nextInput.time);
	writeDebugText(buffer);
	if (getTicks() - replayStartTime >= nextInput.time) {
		currentReplayInput = nextInput.button;
		return currentReplayInput;
	}
	return INPUT_NONE;
}

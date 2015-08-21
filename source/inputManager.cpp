#include "defs.h"
#include "inputDef.h"
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
unsigned long getTicks();
int determineInput(bool mouse);
void cleanup();
extern bool playerDead, displayName, won;
extern int currentLevelNum, levelChange, lastInput;
extern unsigned long lastInputTime, levelStartTime;
extern std::string startLevelName;
int currentInput;
extern bool debugOn;

void recordMove();
bool loadMoves();
int getNextReplayMove(int index);

bool replayEnabled = false;

//Store the latest input so we don't end up reading from the controllers
//multiple times per frame
int getInput(int index = 0) {
	if (replayEnabled) {
		currentInput = getNextReplayMove(index);
	}
	return currentInput;
}
void handleInput() {
	currentInput = determineInput(false);
	if (displayName == true)
	{
		if (currentInput == BUTTON_2) {
			replayEnabled = loadMoves();
			displayName = false;
			return;
		}
		else if (currentInput != INPUT_NONE && (getTicks() - levelStartTime > 1000))
		{
			displayName = false;
		}
		else {
			currentInput = INPUT_NONE;
			return;
		}
	}
	else if (won == true)
	{
		if (currentLevelNum >= MAX_LEVEL - 1)
			exit(0);
		levelChange = currentLevelNum + 1;
		startLevelName.assign("");
	}
	else if (playerDead == true && !replayEnabled)
	{
		int input = getInput();
		if (input != INPUT_NONE)
		{
			if (getTicks() - lastInputTime > 1000 || input != lastInput)
			{
				playerDead = false;
				levelChange = currentLevelNum;
			}
		}
	}
	switch (currentInput) {
	case BUTTON_MENU:
		cleanup();
		break;
	case BUTTON_LEVEL_NEXT:
		if (currentLevelNum < MAX_LEVEL - 1 && (getTicks() - levelStartTime > 1000))
		{
			lastInput = BUTTON_LEVEL_NEXT;
			levelChange = currentLevelNum + 1;
			startLevelName.assign("");
			return;
		}
		lastInput = BUTTON_LEVEL_NEXT;
		break;
	case BUTTON_LEVEL_PREV:
		if (currentLevelNum > 1 && (getTicks() - levelStartTime > 1000))
		{
			lastInput = BUTTON_LEVEL_PREV;
			levelChange = currentLevelNum - 1;
			startLevelName.assign("");
			return;
		}
		lastInput = BUTTON_LEVEL_PREV;
		break;
	case BUTTON_1:
		debugOn = !debugOn;
	}
	if (currentInput != INPUT_NONE) {
		lastInput = currentInput;
		lastInputTime = getTicks();
	}
}
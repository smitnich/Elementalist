#include "defs.h"
#include "inputDef.h"
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "level.h"
#include "inputManager.h"

unsigned long getTicks();
int determineInput(bool mouse);
void cleanup();
extern bool playerDead, displayName, won;
extern int currentLevelNum, levelChange, lastInput;
extern unsigned long lastInputTime, levelStartTime;
extern std::string startLevelName;
int currentInput;
extern bool debugOn;

bool replayEnabled = false;
static const int ticksRequired = 1000;
static int startHold = 0;
//Store the latest input so we don't end up reading from the controllers
//multiple times per frame
int getInput(int index = 0) {
	if (replayEnabled) {
		currentInput = getNextReplayMove(index);
	}
	return currentInput;
}
void handleInput() {
	static int _lastInput = INPUT_NONE;
	currentInput = determineInput(false);
	if (currentInput == BUTTON_RESET || currentInput == BUTTON_MENU
		|| currentInput == BUTTON_LEVEL_NEXT || currentInput == BUTTON_LEVEL_PREV)
	{
		if (_lastInput == currentInput)
		{
			if (getTicks() - ticksRequired < startHold)
				currentInput = INPUT_NONE;
		}
		else
		{
			startHold = getTicks();
			_lastInput = currentInput;
			currentInput = INPUT_NONE;
		}
	}
	else
	{
		startHold = getTicks();
		_lastInput = currentInput;
	}
	if (displayName == true)
	{
		if (currentInput == BUTTON_2) {
#ifdef DEBUG
			replayEnabled = loadMoves();
			displayName = false;
			return;
#endif
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
#ifdef DEBUG
		debugOn = !debugOn;
#endif
		break;
	case BUTTON_RESET:
		restartLevel();
		break;
	default:
		break;
	}
	if (currentInput != INPUT_NONE) {
		lastInput = currentInput;
		lastInputTime = getTicks();
	}
}
void cleanup();
int checkSolid(int, int);
int checkSolid(int, int, int, int);
void switchLevel(int);
void clearObjects();
void objectOutput();
extern float fpsModifier;
extern int frame;
extern int lastInputFrame;
//Move the player
//Todo: Make the player an object
void move()
{
	if (movefraction > 0)
	{
		movefraction+=movespeed;
		if (movefraction >= 48)
		{
			movefraction = 0;
			lastMoveDir = moveDir;
			if (moveDir == D_UP)
				posY -= 1;
			else if (moveDir == D_DOWN)
				posY += 1;
			else if (moveDir == D_LEFT)
				posX -= 1;
			else if (moveDir == D_RIGHT)
				posX += 1;
			moveDir = 0;
		}
	}
	else if (displayName == 1)
	{
		int input = getInput();
		if (input != INPUT_NONE && input != lastInput)
		{
			if (won == true)
			{
				if (currentLevel >= MAX_LEVEL-1)
					exit(0);
				else
					currentLevel++;
				switchLevel(currentLevel);
				startLevelName.assign("");
			}
			if (levelStartCounter == 0)
				displayName = 0;
		}
	}
	else
	{
		int temp = getInput();
		if (temp != 0)
			lastInputFrame = frame;
		switch (temp){
		case BUTTON_LEVEL_NEXT:
			if (currentLevel < MAX_LEVEL-1 && lastInput != BUTTON_LEVEL_NEXT)
			{
				currentLevel++;
				switchLevel(currentLevel);
				startLevelName.assign("");
				//outputLog(lastInput);
			}
			lastInput = BUTTON_LEVEL_NEXT;
			break;
		case BUTTON_LEVEL_PREV:
			if (currentLevel > 1 && lastInput != BUTTON_LEVEL_PREV)
			{
				currentLevel--;
				switchLevel(currentLevel);
				startLevelName.assign("");
			}
			lastInput = BUTTON_LEVEL_PREV;
			break;
		case BUTTON_1:
			if (lastInput != BUTTON_1)
			{
				lastInput = BUTTON_1;
			}
			break;
		case B_RIGHT:
			if (posX < MAP_SIZE && movefraction == 0)
			{
				if (checkSolid(posX,posY,1,0) == 0 || (checkSolid(posX,posY,1,0) == 4 && checkSolid(posX+1,posY,1,0) == 0))
				{
					moveDir = D_RIGHT;
					movefraction = movespeed;
					lastInputFrame = frame;
				}
				else
					lastMoveDir = D_RIGHT;
			}
			lastInput = BUTTON_1;
			break;
		case B_LEFT:
			if (posX > 0 && movefraction == 0)
			{
				if (checkSolid(posX,posY,-1,0) == 0 || (checkSolid(posX,posY,-1,0) == 4 && checkSolid(posX-1,posY,-1,0) == 0))
				{
					moveDir = D_LEFT;
					movefraction = movespeed;
					lastInputFrame = frame;
				}
				else
					lastMoveDir = D_LEFT;
			}
			lastInput = B_LEFT;
			break;
		case B_DOWN:
			if (posY < MAP_SIZE && movefraction == 0)
			{
				if (checkSolid(posX,posY,0,1) == 0 || (checkSolid(posX,posY,0,1) == 4 && checkSolid(posX,posY+1,0,1) == 0))
				{
					moveDir = D_DOWN;
					movefraction = movespeed;
					lastInputFrame = frame;
				}
				else
					lastMoveDir = D_DOWN;
			}
			lastInput = B_DOWN;
			break;
		case B_UP:
			if (posY > 0 && movefraction == 0)
			{
				if (checkSolid(posX,posY,0,-1) == 0 || (checkSolid(posX,posY,0,-1) == 4 && checkSolid(posX,posY-1,0,-1) == 0))
				{
					moveDir = D_UP;
					movefraction = movespeed;
					lastInputFrame = frame;
				}
				else
					lastMoveDir = D_UP;
			}
			lastInput = B_UP;
			break;
		case BUTTON_MENU:
			cleanup();
			lastInputFrame = frame;
			lastInput = BUTTON_MENU;
			break;
		default:
			//lastInput = 0;
			break;
		}
	}
#ifdef GEKKO
	if (HWButton != -1)
		cleanup();
#endif
}
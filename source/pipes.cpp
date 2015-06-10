#include "objectDef.h"
#include "defs.h"
void addMoveRequest(Object *obj, int x, int y, int checkX, int checkY);
bool requestMove(int x, int y, int xChange, int yChange, Object* obj);
#define PIPE_ID 1013
void doDraw(Object *draw, int moveFractionX, int moveFractionY);
void addRenderQueue(Object *obj);
int reverseDir(int dir) {
	switch (dir){
	case D_LEFT:
		return D_RIGHT;
	case D_RIGHT:
		return D_LEFT;
	case D_DOWN:
		return D_UP;
	case D_UP:
		return D_DOWN;
	default:
		return D_NONE;
	}
}
class Pipe : public Object {
public:
	OBJECT_DECLARATION(Pipe, 0)
	Object *within;
	int dir1, dir2;
	Pipe(int x2, int y2) {
		hovering = false;
		numFrames = 1;
		x = x2;
		y = y2;
		objMoveDir = 0;
		objMoveFraction = 0;
		faceDir = 0;
		prevMove = D_NONE;
		within = NULL;
	}
	Object *clone(int _x, int _y) {
		Object *tmp = new Pipe(_x, _y);
		//Copy all of the contents of this object over to the new one
		memcpy(tmp, this, sizeof(*this));
		tmp->x = _x;
		tmp->y = _y;
		return tmp;
	}
	void doLogic() {
		bool moveCompleted = false;
		if (within != NULL) {
			moveCompleted = within->objMove();
			if (moveCompleted) {
				within = NULL;
			}
		}
	}
	bool requestEntry(Object *other, int dir) {
		if (dir == dir1 || dir == dir2 || within != NULL)
			return false;
		else
			return true;
	}
	void onEnter(Object *other, int _xChange, int _yChange) {
		int moveDir = D_NONE;
		int xChange = 0;
		int yChange = 0;
		moveDir = chooseDirection(reverseDir(other->prevMove));
		within = other;
		within->x = this->x;
		within->y = this->y;
		switch (moveDir) {
		case D_LEFT:
			xChange = -1;
			break;
		case D_RIGHT:
			xChange = 1;
			break;
		case D_UP:
			yChange = -1;
			break;
		case D_DOWN:
			yChange = 1;
			break;
		}
		if (!requestMove(x, y, xChange, yChange, other)) {
			moveDir = reverseDir(other->prevMove);
		}
		within->startMove(moveDir);
	}
	int chooseDirection(int dir) {
		if (dir == dir1)
			return dir2;
		else if (dir == dir2)
			return dir1;
		else
			return D_NONE;
	}
	bool allowEntry() {
		return true;
	}
	void draw(int moveFractionX, int moveFractionY) {
		if (within != NULL)
			doDraw(within, moveFractionX, moveFractionY);
		addRenderQueue(this);
	}
};
SPRITE_STATIONARY(Pipe, NULL)
class PipeNW : Pipe {
public:
	OBJECT_DECLARATION(PipeNW,PIPE_ID)
	PipeNW(int x2, int y2) : Pipe(x2, y2) {
		dir1 = D_UP;
		dir2 = D_LEFT;
	}
};
SPRITE_STATIONARY(PipeNW, "gfx/pipeNW.png")
class PipeSW : Pipe {
public:
	OBJECT_DECLARATION(PipeSW, PIPE_ID+1)
		PipeSW(int x2, int y2) : Pipe(x2, y2) {
		dir1 = D_DOWN;
		dir2 = D_LEFT;
	}
};
SPRITE_STATIONARY(PipeSW, "gfx/pipeSW.png")
class PipeSE : Pipe {
public:
	OBJECT_DECLARATION(PipeSE, PIPE_ID + 2)
		PipeSE(int x2, int y2) : Pipe(x2, y2) {
		dir1 = D_DOWN;
		dir2 = D_RIGHT;
	}
};
SPRITE_STATIONARY(PipeSE, "gfx/pipeSE.png")
class PipeNE : Pipe {
public:
	OBJECT_DECLARATION(PipeNE, PIPE_ID + 3)
		PipeNE(int x2, int y2) : Pipe(x2, y2) {
		dir1 = D_UP;
		dir2 = D_RIGHT;
	}
};
SPRITE_STATIONARY(PipeNE, "gfx/pipeNE.png")


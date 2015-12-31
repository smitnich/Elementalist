#include "imageMacros.h"
#include "tileEnum.h"

class Ash : public Object 
{
	public:
		OBJECT_DECLARATION(Ash, OBJ_ASH)
		Ash(int x, int y) : Object(x, y) {}
		Object *clone(int x, int y)
		{
			return NULL;
		}
		void electrocute()
		{
			return;
		}
		void freeze()
		{
			return;
		}
		void burn()
		{
			return;
		}
		bool requestEntry(Object *other, int dir)
		{
			return true;
		}
		void onCollision(Object *other, int dir)
		{
			die();
		}
		bool allowEntry()
		{
			return true;
		}
};

SPRITE_STATIONARY(Ash, "gfx/ash.png")

class MultipleObjectManager : public Object {
public:
	std::vector<Object *> within;
	MultipleObjectManager(int _x, int _y);
	~MultipleObjectManager();
	void draw(int moveFractionX, int moveFractionY);
	void addObject(Object *in);
	void doLogic();
	void die();
	void loadImages();
	SDL_Surface *getSprite();
	bool requestEntry(Object *other, int dir);
	void onEnter(Object *other, int _x, int _y);
	void onCollision(Object *other, int dir);
	bool objMove();
	Object *clone(int _x, int _y);
	Object *createInstance(int _x, int _y);
	bool allowEntry();
};
MultipleObjectManager *makeObjectManager(int _x, int _y);
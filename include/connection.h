#ifndef CONNECTION
#define CONNECTION

class Terrain;
class Object;

struct connection
{
	Terrain *terrain;
	Object *obj;
	int index;
};

void activateConnection(connection *con);
void deactivateConnection(connection *con);

#endif
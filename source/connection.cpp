#include "objectDef.h"
#include "connection.h"
#include "terrain.h"

void activateConnection(connection *con)
{
	if (con->terrain != NULL)
		con->terrain->activate();
	if (con->obj != NULL)
		con->obj->activate();
	// Check if the object was destroyed by this
	// interaction so that we don't try to use
	// an invalid pointer later
	if (con->obj != NULL && con->obj->dead)
		con->obj = NULL;
}
void deactivateConnection(connection *con)
{
	if (con->terrain != NULL)
		con->terrain->deactivate();
	if (con->obj != NULL)
		con->obj->deactivate();
	// Check if the object was destroyed by this
	// interaction so that we don't try to use
	// an invalid pointer later
	if (con->obj != NULL && con->obj->dead)
		con->obj = NULL;

}
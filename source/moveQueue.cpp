#include "objectDef.h"
#include <list>

struct Request
{
	int xReq;
	int yReq;
	Object *obj;
};
void tryPlace(Request req);
std::list<Request> moveRequests;

void enqueue(Request req)
{
	moveRequests.push_back(req);
	//moveRequests.emplace_back(req);
}
void tryRequests()
{
	for (std::list<Request>::iterator it = moveRequests.begin();
		it != moveRequests.end(); it++)
	{
		tryPlace(*it);
	}
}
void tryPlace(Request req)
{

}
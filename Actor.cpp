#include "GraphObject.h"
#include "Actor.h"
#include "StudentWorld.h"

///////////////////////////////////////////////////////////////
///////////////// GRASSHOPPER IMPLEMENTATION //////////////////
///////////////////////////////////////////////////////////////

// what grasshoppers do during a tick
void Grasshopper::doSomething()
{
	// lose one hit point
	setPoints(getPoints() - 1);

	Direction dir = getDirection();
	int x = getX(), y = getY();

	//for (int )


	// new random direction
	setDirection(randDir());

	// new random distance
	distance = randInt(2, 10);

}

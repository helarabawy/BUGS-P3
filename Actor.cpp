#include "GraphObject.h"
#include "Actor.h"
#include "StudentWorld.h"

///////////////////////////////////////////////////////////////
///////////////// GRASSHOPPER IMPLEMENTATION //////////////////
///////////////////////////////////////////////////////////////

// what grasshoppers do during a tick
void Grasshopper::doSomething()
{
// TODO: make sure no pebbles along path

	// lose one hit point
	points--;

	// if dead
	//if (points <= 0)

	// sleeping
	// fix walking

	Direction dir = getDirection();
	int x = getX(), y = getY();

	//for (int )


	// new random direction
	setDirection(randDir());

	// new random distance
	distance = randInt(2, 10);

}

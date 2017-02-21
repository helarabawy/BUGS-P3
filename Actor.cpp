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
	m_points--;

	Direction dir = getDirection();
	int x = getX(), y = getY();

	//for (int )


	// new random direction
	setDirection(randDir());

	// new random distance
	distance = randInt(2, 10);

}

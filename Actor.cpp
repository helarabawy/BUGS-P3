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
	points--;

	Direction dir = getDirection();
	int x = getX(), y = getY();
	switch (dir)
	{
		case up:
		{
			moveTo(x, y - distance);
			break;
		}

		case right:
		{
			moveTo(x - distance, y);
			break;
		}

		case down:
		{
			moveTo(x, y + distance);
			break;
		}

		case left:
		{
			moveTo(x + distance, y);
			break;
		}

		case none: break;
	}

	// new random direction
	setDirection(randDir());

	// new random distance
	distance = randInt(2, 10);

}


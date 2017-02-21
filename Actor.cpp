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
	ticks++;

	// check if died
	if (isDead() || isSleeping())
		return; // TODO: should move() function deal with dead grasshoppers?

	Direction dir = getDirection();
	int x = getX(), y = getY();

	// TODO: figure out incremental movement
	/*switch (dir)
	{
		case up:
		{
			for (int i = 0; i < distance; i++)
			{

			}
			break;
		}
		case right: {}
		case down: {}
		case left: {}
	}
*/
	// new random direction
	setDirection(randDir());

	// new random distance
	distance = randInt(2, 10);

}

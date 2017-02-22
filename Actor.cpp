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

	cerr << "Points: " << getPoints() << endl;

	//another tick
		ticks++;
	// check if died
	if (isDead() || isSleeping()) {cerr << endl << "DEAD/ASLEEP" << endl; return;}
	 // TODO: should move() function deal with dead grasshoppers?
cerr << "AWAKE" << endl;


	Direction dir = getDirection();

	int oldX = getX();
	int oldY = getY();

	cout << "Dist: " << distanceToMove << endl;
	// make movement based on direction and roadblocks
	switch (dir)
	{
		case up:
		{
			if (!hasPebble(oldX, oldY + 1))
			{
				cerr << "called moveTo up" << endl;
				moveTo(oldX, oldY + 1);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}

		case right:
		{
			if (!hasPebble(oldX + 1, oldY))
			{
				cerr << "called moveTo right" << endl;
				moveTo(oldX + 1, oldY);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		case down:
		{
			if (!hasPebble(oldX, oldY - 1))
			{
				cerr << "called moveTo down" << endl;
				moveTo(oldX, oldY - 1);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		case left:
		{
			if (!hasPebble(oldX - 1, oldY))
			{
				cerr << "called moveTo left" << endl;
				moveTo(oldX - 1, oldY);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		default:
		{
			cerr << "should never reach here" << endl;
		}
	}

	cerr << "after moving: " << distanceToMove << endl;
	if (distanceToMove == 0)
	{
		// new random direction
		setDirection(randDir());

		// new random distance
		distanceToMove = randInt(2, 10);
	}
}

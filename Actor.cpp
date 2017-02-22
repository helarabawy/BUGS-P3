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

	//another tick
	ticks++;

	// check if died
	if (isDead() || isSleeping())
		return;

	// current direction
	Direction dir = getDirection();

	// current x, y
	int oldX = getX();
	int oldY = getY();

	// make movement based on direction and roadblocks
	switch (dir)
	{
		case up:
		{
			if (!hasPebble(oldX, oldY + 1))
			{
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
				moveTo(oldX - 1, oldY);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
	}

	if (distanceToMove <= 0)
	{
		// new random direction
		setDirection(randDir());

		// new random distance
		distanceToMove = randInt(2, 10);
	}
}

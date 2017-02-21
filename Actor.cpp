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
	int oldX = getX();
	int oldY = getY();

	// TODO: figure out incremental movement
	// TODO: HOW TO CHECK IF THERE IS A PEBBLE
	for (int i = 0; i < distance; i++)
	{
		switch (dir)
		{
			case up:
			{
				if (!hasPebble(oldX, oldY - i))
					moveTo(oldX, oldY - i);
				else
					i = distance;
				break;
			}
			case right:
			{
				if (!hasPebble(oldX + i, oldY))
					moveTo(oldX + i, oldY);
				else
					i = distance;
				break;
			}
			case down:
			{
				if (!hasPebble(oldX, oldY + i))
					moveTo(oldX, oldY + i);
				else
					i = distance;
				break;
			}
			case left:
			{
				if (!hasPebble(oldX - i, oldY))
					moveTo(oldX - i, oldY);
				else
					i = distance;
				break;
			}
		}
	}


	// new random direction
	setDirection(randDir());

	// new random distance
	distance = randInt(2, 10);

}

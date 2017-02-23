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

	/*if (isStunned())
	{
		decrementStunnedTicks();
		return;
	}
	unstun();*/

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
			if (!isBlocked(oldX, oldY + 1))
			{
				moveTo(oldX, oldY + 1);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}

		case right:
		{
			if (!isBlocked(oldX + 1, oldY))
			{
				moveTo(oldX + 1, oldY);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		case down:
		{
			if (!isBlocked(oldX, oldY - 1))
			{
				moveTo(oldX, oldY - 1);
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		case left:
		{
			if (!isBlocked(oldX - 1, oldY))
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

GraphObject::Direction Grasshopper::randDir()
{
	int rand = randInt(1, 4);
	switch(rand)
	{
		case 1: return GraphObject::up;
		case 2: return GraphObject::right;
		case 3: return GraphObject::down;
		case 4: return GraphObject::left;
		default:return GraphObject::none; // TODO: check if none should be an option
	}
}

///////////////////////////////////////////////////////////////
////////////// BABY GRASSHOPPER IMPLEMENTATION ////////////////
///////////////////////////////////////////////////////////////

bool BabyGrasshopper::isStunned()
{
	if (checkStunStatus() == true && stunnedTicks == 0)
	{
		stunnedTicks = 2;
		return true;
	} else if (checkStunStatus() == true && stunnedTicks == 1)
	{
		return true;
	}
	 else if (stunnedTicks == -1)
	 {
		stunnedTicks = 0;
		return false;
	 } else
		return false;
}

void BabyGrasshopper::decrementStunnedTicks()
{
	if (stunnedTicks == 1)
		stunnedTicks -= 2;
	else stunnedTicks--;
}



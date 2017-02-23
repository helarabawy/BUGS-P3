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

	cout << "1. check stun status: " <<  checkStunStatus() << endl;
	if (isStunned())
	{
		decrementStunnedTicks();
		return;
	}
	unstun();

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



///////////////////////////////////////////////////////////////
////////////// BABY GRASSHOPPER IMPLEMENTATION ////////////////
///////////////////////////////////////////////////////////////

bool BabyGrasshopper::isStunned()
{

	cerr << "2. check stun status: " << checkStunStatus() << endl;
	if (checkStunStatus() == true && stunnedTicks == 0)
	{
		stunnedTicks = 2;
		cerr << "is stunned" << endl;
		return true;
	} else if (checkStunStatus() == true && stunnedTicks == 1)
	{
		cerr << "is stunned" << endl;
		return true;
	}
	 else if (stunnedTicks == -1)
	 {
		 cerr << "not stunned" << endl;
		stunnedTicks = 0;
		return false;
	 } else
		return false;
}


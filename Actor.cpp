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

	// check if died
	if (isDead() || isSleeping())
		return;

	// DIFFERENTIATED FUNCTION
	doFunction();

	// eat
	eat();

	// current direction
	Direction dir = getDirection();

	// current x, y
	int oldX = getX();
	int oldY = getY();

	moveStep(dir, oldX, oldY);


}

bool Grasshopper::isSleeping()
		{
			if (checkStunStatus() == false)
			{
				if (ticksToSleep == 0)
				{
					ticksToSleep = 2;
					stunned = false;
					return false;
				} else
				{
					ticksToSleep--;
					return true;
				}
			}
			else
			{
				ticksToSleep = 9;
				return true;
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

void Grasshopper::moveStep(GraphObject::Direction dir, int oldX, int oldY)
{
	// make movement based on direction and roadblocks
	switch (dir)
	{
		case up:
		{
			if (!isBlocked(oldX, oldY + 1))
			{
				moveTo(oldX, oldY + 1); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX, oldY + 1); // move pointers
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}

		case right:
		{
			if (!isBlocked(oldX + 1, oldY))
			{
				moveTo(oldX + 1, oldY); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX + 1, oldY); // move pointers
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		case down:
		{
			if (!isBlocked(oldX, oldY - 1))
			{
				moveTo(oldX, oldY - 1);// move on grid
				m_game->moveActor(this, oldX, oldY, oldX, oldY - 1); // move pointers
				distanceToMove--;
			}
			else {distanceToMove = 0;}

			break;
		}
		case left:
		{
			if (!isBlocked(oldX - 1, oldY))
			{
				moveTo(oldX - 1, oldY); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX - 1, oldY); // move pointers
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

void BabyGrasshopper::doFunction()
{
	grow();
}

void BabyGrasshopper::grow()
{
	if (getPoints() >= 1600)
		m_game->growGrasshopper(this, getX(), getY());
}

///////////////////////////////////////////////////////////////
///////////// ADULT GRASSHOPPER IMPLEMENTATION ////////////////
///////////////////////////////////////////////////////////////

void AdultGrasshopper::doFunction()
{
	bite();
	jump();
}

void AdultGrasshopper::bite()
{

}

void AdultGrasshopper::jump()
{

}

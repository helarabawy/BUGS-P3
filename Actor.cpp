#include "GraphObject.h"
#include "Actor.h"
#include "StudentWorld.h"

///////////////////////////////////////////////////////////////
/////////////// ANIMATE ACTOR IMPLEMENTATION //////////////////
///////////////////////////////////////////////////////////////

void AnimateActor::doSomething()
{
	// lose one hit point
	setPoints(getPoints() - 1);
}

GraphObject::Direction AnimateActor::randDir()
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
///////////////// GRASSHOPPER IMPLEMENTATION //////////////////
///////////////////////////////////////////////////////////////

// what grasshoppers do during a tick
void Grasshopper::doSomething()
{
	AnimateActor::doSomething();

	// check if died
	if (isDead() || isSleeping())
		return;

	// DIFFERENTIATED FUNCTION
	doFunction();

	// eat
	//eat();

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
			return false;
		} else
		{
			ticksToSleep--;
			return true;
		}
	}
	else
	{
		if (recoveringFromStun == false)
		{
			recoveringFromStun = true;
			ticksToSleep = 9;
			return true;
		} else if (recoveringFromStun == true)
		{
			ticksToSleep--;
			if (ticksToSleep == 0)
			{
				unstun();
				recoveringFromStun = false;
			}
			return true;
		}
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
	// 1/3 chances to bite
	if (randInt(1,3) == 1)
		bite();

	// 1/10 chances to jump
	if (randInt(1, 10) == 1)
		jump();
}

void AdultGrasshopper::bite()
{
	// DO STUFF
}

const int TOTAL_POSSIBLE_SLOTS = 100; // WRONG, FIGURE OUT THIS NUMBER
#include <cmath>
const double PI = 3.1415926535897;
void AdultGrasshopper::jump()
{
	// no more options left
	if (jumpOptions.size() == TOTAL_POSSIBLE_SLOTS)
		return;

	int radius = randInt(1, 10);
	int angle = randInt(0, 364);

	// defining x & y
	int x = ceil(radius*cos(angle*PI/180));
	int y = ceil(radius*sin(angle*PI/180));

	// checking if slot is open
	if (m_game->isBlocked(x, y))
	{
		Coord c;
		c.x = x;
		c.y = y;

		for (int i = 0; i < jumpOptions.size(); i++)
		{
			// found an already checked coordinate
			if (jumpOptions[i].x == c.x && jumpOptions[i].y == c.y)
				jump();
		}
		// no repeated coordinates, store this one
		jumpOptions.push_back(c);
		jump();
	} else
		jumpTo(x, y); // found a place to jump to
}

void AdultGrasshopper::jumpTo(int x, int y)
{
	m_game->moveActor(this, getX(), getY(), x, y);
	moveTo(x, y);

}


///////////////////////////////////////////////////////////////
////////////////////// ANT IMPLEMENTATION /////////////////////
///////////////////////////////////////////////////////////////

bool Ant::isSleeping()
{
	if (checkStunStatus() == true)
	{
		if (ticksToSleep == 0)
		{
			ticksToSleep = 2;
			unstun();
			return false;
		} else
		{
			ticksToSleep--;
			return true;
		}
	}
	else
		return false;
}

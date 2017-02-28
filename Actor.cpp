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

bool AnimateActor::eat(int maxFood)
{
	// is there food? if so, eat
	int foodPts = m_game->eatFood(getX(), getY(), maxFood);

	if (foodPts == 0) // no food to eat
		return false;
	else
		setPoints(getPoints() + foodPts);

	return true;
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
	if (doFunction() == true)
	{
		return; // go into sleep
	}

	// eat
	if (eat() == true)
	{
		return; // go into sleep
	}

	// current direction
	Direction dir = getDirection();

	// current x, y
	int oldX = getX();
	int oldY = getY();

	moveStep(dir, oldX, oldY);
	

	if (distanceToMove <= 0)
	{
		// new random direction
		setDirection(randDir());

		// new random distance
		distanceToMove = randInt(2, 10);
	}


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
			ticksToSleep = 8;
			return true;
		} else if (recoveringFromStun == true)
		{
			ticksToSleep--;
			if (ticksToSleep == 0)
			{
				unstun();
				ticksToSleep = 2;
				recoveringFromStun = false;
				return false;
			}
			return true;
		}
	}
}



bool Grasshopper::moveStep(GraphObject::Direction dir, int oldX, int oldY)
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
				return true;
			}
			else {distanceToMove = 0; return false;}

			break;
		}

		case right:
		{
			if (!isBlocked(oldX + 1, oldY))
			{
				moveTo(oldX + 1, oldY); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX + 1, oldY); // move pointers
				distanceToMove--;
				return true;
			}
			else {distanceToMove = 0; return false;}

			break;
		}
		case down:
		{
			if (!isBlocked(oldX, oldY - 1))
			{
				moveTo(oldX, oldY - 1);// move on grid
				m_game->moveActor(this, oldX, oldY, oldX, oldY - 1); // move pointers
				distanceToMove--;
				return true;
			}
			else {distanceToMove = 0; return false;}

			break;
		}
		case left:
		{
			if (!isBlocked(oldX - 1, oldY))
			{
				moveTo(oldX - 1, oldY); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX - 1, oldY); // move pointers
				distanceToMove--;
				return true;
			}
			else {distanceToMove = 0; return false;}

			break;
		}
	}
}

///////////////////////////////////////////////////////////////
////////////// BABY GRASSHOPPER IMPLEMENTATION ////////////////
///////////////////////////////////////////////////////////////

bool BabyGrasshopper::doFunction()
{
	if (getPoints() >= 1600)
	{
		m_game->growGrasshopper(this, getX(), getY());
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////
///////////// ADULT GRASSHOPPER IMPLEMENTATION ////////////////
///////////////////////////////////////////////////////////////

bool AdultGrasshopper::doFunction()
{
	// TODO: REVIEW LOGIC
	// 1/3 chances to bite
	if (randInt(1,3) == 1)
		if (m_game->biteRandomInsect(getX(), getY()) == true)
			return true;

	// 1/10 chances to jump
	if (randInt(1, 10) == 1)
		jump();


	// 50% chance to sleep if ate
	if (randInt(1,2) == 1)
		return true;
	else
		return false;


	return jumped;
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
	jumped = true;
	m_game->moveActor(this, getX(), getY(), x, y);
	moveTo(x, y);

}


///////////////////////////////////////////////////////////////
////////////////////// ANT IMPLEMENTATION /////////////////////
///////////////////////////////////////////////////////////////

void Ant::doSomething()
{
	AnimateActor::doSomething();
	
	// check if died
	if (isDead() || isSleeping())
		return;
	
	doFunction();
	
}

void Ant::storeFood(int amount)
{
	storedFood += m_game->eatFood(getX(), getY(), 400);
}
void Ant::doFunction() 
{
	int ic = 0; // instruction counter
	
	if (!c.getCommands(ic, cmd))
		return false;
	
	switch (cmd.operator)
	{
		case moveForward: // DONE
		{
			bool gotBlocked = moveTo(getDirection, getX(), getY());
			++ic;
			break;
		}
		
		case eatFood: // DONE
		{
			eat(100);
			++ic;
			break;
		}
		
		case dropFood: // DONE
		{
			m_game->dropFood(getX(), getY(), storedFood); // TODO: make sure to define getFoodPts
			++ic;
			break;
		}

		case bite: // DONE
		{
			m_game->biteRandomInsect(getX(), getY(), 15);
			++ic;
			break;
		}

		case pickupFood:
		{
			storeFood(400);
			++ic;
			break;
		}
		case emitpheromone:
		{
			//
			++ic;
			break;
		}
		case generateRandomNumber:
		{
			generatorRandomNumberUpTo(cmd.operand1);
			++ic;
			break;
		}
		
		case faceRandomDirection: // DONE
		{
			setDirection(randDir());
			++ic;
			break;
		}
		
		case rotateClockwise:
		{
			//
			++ic;
			break;
		}
		
		case rotateCounterClockwise:
		{
			//
			++ic;
			break;
		}
		
		case if_command:
		{
			//
			++ic;
			break;
		}
		case goto_command:
		{
			//
			++ic;
			break;
		}
		
		case generateRandomNumber:
		{
			//
			++ic;
			break;
		}
		
		case label:
		{
			//
			++ic;
			break;
		}
	
		default:
		{
			setPoints(0);
			break;
		}
	}
}


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


///////////////////////////////////////////////////////////////
//////////////////// ANTHILL IMPLEMENTATION ///////////////////
///////////////////////////////////////////////////////////////

void Anthill::doSomething()
{
	DecayableActor::doSomething();
	
	// check if died
	if (isGone())
		return;
	
	doFunction();
	
}

void Anthill::doFunction() 
{
	// check to see if there is any food on square
	// eat up to 10,000 units of food -----------------------not sure how to do this
	// immediately return
	
	
	// no food? check if there is enough energy (>= 2000 hit points to produce new ant
	// add new ant on square
	// lose 1500 points
	// ask student world ot increase count of total number of ants that this colony has produced (to see who is winning)
	if (getPoints() >= 2000)
	{
		m_game->newAntBorn(getX(), getY(), getColony());
		setPoints(getPoints() - 1500);
	}
	
}

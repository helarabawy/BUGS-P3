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

// Generating random direction
GraphObject::Direction AnimateActor::randDir()
{
	int rand = randInt(1, 4);
	switch(rand)
	{
		case 1: return GraphObject::up;
		case 2: return GraphObject::right;
		case 3: return GraphObject::down;
		case 4: return GraphObject::left;
		default:return GraphObject::none;
	}
}

// eating
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

// poisoning
void AnimateActor::poison()
{
	if (poisoned == false)
	{
		m_points -= 150;
		poisoned = true;
	} else
		return;
}


// put this here to move to animate actor eventually
bool AnimateActor::moveStep(GraphObject::Direction dir, int oldX, int oldY)
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
				setDistanceToMove(getDistanceToMove() - 1);
				return true;
			}
			else {setDistanceToMove(0); return false;}

			break;
		}

		case right:
		{
			if (!isBlocked(oldX + 1, oldY))
			{
				moveTo(oldX + 1, oldY); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX + 1, oldY); // move pointers
				setDistanceToMove(getDistanceToMove() - 1);
				return true;
			}
			else {setDistanceToMove(0); return false;}

			break;
		}
		case down:
		{
			if (!isBlocked(oldX, oldY - 1))
			{
				moveTo(oldX, oldY - 1);// move on grid
				m_game->moveActor(this, oldX, oldY, oldX, oldY - 1); // move pointers
				setDistanceToMove(getDistanceToMove() - 1);
				return true;
			}
			else {setDistanceToMove(0); return false;}

			break;
		}
		case left:
		{
			if (!isBlocked(oldX - 1, oldY))
			{
				moveTo(oldX - 1, oldY); // move on grid
				m_game->moveActor(this, oldX, oldY, oldX - 1, oldY); // move pointers
				setDistanceToMove(getDistanceToMove() - 1);
				return true;
			}
			else {setDistanceToMove(0); return false;}

			break;
		}
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
	if (doFunction() == true)
	{
		return; // go into sleep
	}

	// eat
	if (eat(200) == true)
	{
		// 50% chance of going to sleep
		if (randInt(0, 1) == 1)
			return; // go into sleep
	}

	// current direction
	Direction dir = getDirection();

	// current x, y
	int oldX = getX();
	int oldY = getY();

	moveStep(dir, oldX, oldY);
	

	if (getDistanceToMove() <= 0)
	{
		// new random direction
		setDirection(randDir());

		// new random distance
		setDistanceToMove(randInt(2, 10));
	}


}

// to check if grasshopper is sleeping
bool Grasshopper::isSleeping()
{
	if (checkStunStatus() == false)
	{
		// awake
		if (ticksToSleep == 0)
		{
			ticksToSleep = 2;
			return false;
		} else // asleep
		{
			ticksToSleep--;
			return true;
		}
	}
	else
	{
		// stunned sleep
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

///////////////////////////////////////////////////////////////
////////////// BABY GRASSHOPPER IMPLEMENTATION ////////////////
///////////////////////////////////////////////////////////////

// baby grasshopper differentiated function
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

// adult grasshopper differentiated function
bool AdultGrasshopper::doFunction()
{
	// return true if it did a thing and should sleep right away

	// 1/3 chances to bite
	if (randInt(1,3) == 1)
		if (m_game->biteRandomInsect(this, getX(), getY(), 50) == true)
			return true;

	// 1/10 chances to jump
	if (randInt(1, 10) == 1)
	{
		jump();
		return true;
	}

	// did not do anything
	return false;
}


#include <cmath>
const double PI = 3.1415926535897;
bool AdultGrasshopper::jump()
{
	bool jumped = false;
	int trialCount = 0;

	// trying to jump
	while (!jumped && trialCount <= 1000)
	{
		int radius = randInt(1, 10);
		int angle = randInt(0, 364);

		// defining x & y
		int dx = ceil(radius*cos(angle*PI/180));
		int dy = ceil(radius*sin(angle*PI/180));

		// new x and new y
		int newX = getX() + dx;
		int newY = getY() + dy;

		// checking if slot is open
		if (!m_game->isBlocked(newX, newY) && isInBounds(newX, newY))
		{
			jumped = true;
			jumpTo(newX, newY); // found a place to jump to
		} else
			trialCount++;
	}
	return jumped;

}

// function for adult grasshopper to jump
void AdultGrasshopper::jumpTo(int x, int y)
{
	m_game->moveActor(this, getX(), getY(), x, y);
	moveTo(x, y);
}

// checking if locaation is in bounds
bool AdultGrasshopper::isInBounds(int x, int y)
{
	return (x < 64 && x > 0 && y < 64 && y > 0);
}

///////////////////////////////////////////////////////////////
////////////////////// ANT IMPLEMENTATION /////////////////////
///////////////////////////////////////////////////////////////

void Ant::doSomething()
{
	AnimateActor::doSomething();
	
	// check if died
	if (isDead())
	{
		m_game->decrementAntCount(getColony());
		return;
	}

	// check if sleeping
	if (isSleeping())
		return;
	
	doFunction();
	
}

// for ant to sotre food
void Ant::storeFood(int amount)
{
	int toStore = m_game->eatFood(getX(), getY(), 400);

	// max pts of 1800
	if (storedFood + toStore > 1800)
	{
		storedFood = 1800;
		m_game->dropFood(this, getX(), getY(), storedFood + toStore - 1800);
	} else
	{
		storedFood = storedFood + toStore;
	}

}

// ant's differentiated function
void Ant::doFunction()
{
	// reset all variables
	int commandCount = 0;
	int rand;
	string::size_type sz;
	bool gotBlocked = false;
	Compiler::Command cmd;
	
	while (commandCount <= 10)
	{

		if (!m_compiler->getCommand(ic, cmd))
		{
			setPoints(0);
			return; // there was an error
		}

		commandCount++;
		switch (cmd.opcode) // following commands
		{
			case Compiler::Opcode::moveForward:
			{
				bitten = false;
				gotBlocked = moveStep(getDirection(), getX(), getY());
				ic++;
				return;
			}

			case Compiler::Opcode::eatFood:
			{
				eat(100);
				ic++;
				return;
			}

			case Compiler::Opcode::dropFood:
			{
				m_game->dropFood(this, getX(), getY(), storedFood);
				ic++;
				return;
			}

			case Compiler::Opcode::bite:
			{
				m_game->biteRandomInsect(this, getX(), getY(), 15);
				ic++;
				return;
			}

			case Compiler::Opcode::pickupFood:
			{
				storeFood(400);
				ic++;
				return;
			}

			case Compiler::Opcode::emitPheromone:
			{
				m_game->emitPheromone(getX(), getY(), getColony());
				ic++;
				return;
			}

			case Compiler::Opcode::faceRandomDirection:
			{
				setDirection(randDir());
				ic++;
				return;
			}

			case Compiler::Opcode::generateRandomNumber:
			{
				int op1 = stoi(cmd.operand1, &sz);
				if (op1 != 0)
					rand = randInt(0, op1 - 1);
				else
					rand = 0;
				ic++;
				break;
			}

			case Compiler::Opcode::rotateClockwise:
			{
				rotateClockwise();
				ic++;
				return;
			}

			case Compiler::Opcode::rotateCounterClockwise:
			{
				rotateCounterClockwise();
				ic++;
				return;
			}
			// GOTO COMMAND
			case Compiler::Opcode::goto_command:
			{

				int op1 = stoi(cmd.operand1, &sz);
				ic = op1;
				break;
			}
			// IF COMMAND, ADJUSTING IC
			case Compiler::Opcode::if_command:
			{
				int op1 = stoi(cmd.operand1, &sz);
				int op2 = stoi(cmd.operand2, &sz);
				switch (op1)
				{
					case Compiler::Condition::last_random_number_was_zero:
					{
						if (rand == 0)
							ic = op2;
						break;
					}
					case Compiler::Condition::i_am_carrying_food:
					{
						if (storedFood > 0)
							ic = op2;
						break;
					}
					case Compiler::Condition::i_am_hungry:
					{
						if (getPoints() <= 25)
							ic = op2;
						break;
					}

					case Compiler::Condition::i_am_standing_with_an_enemy:
					{
						if (m_game->hasEnemy(getX(), getY(), getColony()))
							ic = op2;
						break;
					}

					case Compiler::Condition::i_am_standing_on_food:
					{
						if (m_game->hasFood(getX(), getY()) != nullptr)
							ic = op2;
						break;
					}

					case Compiler::Condition::i_am_standing_on_my_anthill:
					{
						if (getX() == m_game->getColonyX(m_colony) &&
							getY() == m_game->getColonyY(m_colony))
							ic = op2;
						break;
					}

					case Compiler::Condition::i_smell_pheromone_in_front_of_me:
					{
						// looking for pheromone in front of ant
						switch (getDirection())
						{
							case GraphObject::up:
							{
								if (m_game->hasPheromone(getX(), getY() + 1, getColony()))
									ic = op2;
								break;
							}
							case GraphObject::down:
							{
								if (m_game->hasPheromone(getX(), getY() - 1, getColony()))
									ic = op2;
								break;
							}
							case GraphObject::right:
							{
								if (m_game->hasPheromone(getX() + 1, getY(), getColony()))
									ic = op2;
								break;
							}
							case GraphObject::left:
							{
								if (m_game->hasPheromone(getX() - 1, getY() + 1, getColony()))
									ic = op2;
								break;
							}
						}
						break;
					}

					case Compiler::Condition::i_was_bit:
					{
						if (bitten == true)
							ic = op2;
						break;
					}

					case Compiler::Condition::i_was_blocked_from_moving:
					{
						if (gotBlocked)
							ic = op2;
						break;
					}

				}
				ic++;
				break;
			}
			default:
			{
				// if none of these, then ant is dead
				setPoints(0);
				break;
			}
		}

	}

}

// CHECKING IF ANT IS SLEEPING
bool Ant::isSleeping()
{
	if (checkStunStatus() == true)
	{
		if (ticksToSleep == 0)
		{
			ticksToSleep = 2;
			unstun();
			bitten = false; // ???
			return false;
		} else
		{
			bitten = false; // ???
			ticksToSleep--;
			return true;
		}
	}
	else
		return false;
}
// ROTATE CLOCKWISE
void Ant::rotateClockwise()
{
	GraphObject::Direction dir = getDirection();


	switch(dir)
	{
		case GraphObject::up:
		{
			setDirection(GraphObject::right);
			break;
		}
		case GraphObject::right:
		{
			setDirection(GraphObject::down);
			break;
		}
		case GraphObject::down:
		{
			setDirection(GraphObject::left);
			break;
		}
		case GraphObject::left:
		{
			setDirection(GraphObject::up);
			break;
		}
		default: break;
	}
}
// ROTATE COUNTERCLOCKWISE
void Ant::rotateCounterClockwise()
{
	GraphObject::Direction dir = getDirection();

	switch(dir)
	{
		case GraphObject::up:
		{
			setDirection(GraphObject::left);
			break;
		}
		case GraphObject::right:
		{
			setDirection(GraphObject::up);
			break;
		}
		case GraphObject::down:
		{
			setDirection(GraphObject::right);
			break;
		}
		case GraphObject::left:
		{
			setDirection(GraphObject::down);
			break;
		}
		default: break;
	}
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
	// eat up to 10,000 units of food
	if (m_game->eatFood(getX(), getY(), 10000) > 0)
		return;
	
	// no food? check if there is enough energy (>= 2000 hit points to produce new ant
	// add new ant on square
	// lose 1500 points
	// ask student world it increase count of total number of ants that this colony has produced (to see who is winning)
	if (getEnergy() >= 2000)
	{
		m_game->newAntBorn(getX(), getY(), getColony(), m_c);
		setEnergy(getEnergy() - 1500);
	}
	
}

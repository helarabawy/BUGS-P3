// Packages/Header Files to include
#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////
////////////////////// SIMULATION CONTROL /////////////////////
///////////////////////////////////////////////////////////////

// CONSTRUCTOR
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// STARTING SIMULATION
int StudentWorld::init()
{
	// Initialize ticks to 0
	currTicks = 0;

	// Making sure field/entrant entries load/compile correctly
	if (!compileAntPrograms() || !loadField())
		{
			return GWSTATUS_LEVEL_ERROR;
		}
	else
		{
		
			return GWSTATUS_CONTINUE_GAME;
		}
}

// DOING EACH MOVE
int StudentWorld::move()
{
	// New round, increment ticks
	currTicks++;

	// Give each actor a chance to do something
	for(int i = 0; i < VIEW_WIDTH*VIEW_HEIGHT; i++)
	{
		 // defining iterator at (x, y)'s list of Actor*
		 list<Actor*>::const_iterator it = virtualWorld[i].begin();

		 // calling all actors at (x, y) to do sth
		 while(it != virtualWorld[i].end())
		 {
			 (*it)->doSomething();

			 // remove dead actors
			 it = removeDeadActorsAndGetNext(it, i);
		 }
	}
	
	// perform all pointer redirecting
	redirectActorPtrs();
	
	// clear movables after everything moved
	toMove.clear();

	updateDisplayText();

	// game ended
	if (currTicks == 2000)
	{
		if (getWinner() == -1)
			return GWSTATUS_NO_WINNER;
		else
			return GWSTATUS_PLAYER_WON;
	}
	
	// continue simulation
	return GWSTATUS_CONTINUE_GAME;
	
	// TODO: KEEP TRACK OF SCORE AND RETURN WINNER!
}

// CLEAN UP SIMULATION
void StudentWorld::cleanUp()
{
	for (int i = 0; i < VIEW_WIDTH*VIEW_HEIGHT; i++)
	{
		// defining iterator at (x, y)'s list of Actor*
		list<Actor*>::const_iterator it;

		// calling all actors at (x, y) to do sth
		for (it = virtualWorld[i].begin(); it != virtualWorld[i].end(); it++)
		{
			 // clear actor
			 delete *it;
			 virtualWorld[i].erase(it);

			 // redirect iterator to look for other actors to clear
			 it = virtualWorld[i].begin();
		}
	}
}

// COMPILE ANT PROGRAMS
#include "Compiler.h"
bool StudentWorld::compileAntPrograms()
{
	// Max num of entrants
	Compiler *compilerForEntrant0, *compilerForEntrant1, 
			 *compilerForEntrant2, *compilerForEntrant3;
	
	// anthills
	Anthill *ah0, *ah1, *ah2, *ah3;
	
	vector<string> filenames = getFilenamesOfAntPrograms();
	string error;
	
	// ENTRANT 0 
	if (filenames.size() >= 1)
	{
		compilerForEntrant0 = new Compiler;
		compiledEntrants.push_back(compilerForEntrant0);
	
		// spot and set error
		if (!compilerForEntrant0->compile(filenames[0], error))
		{	
			setError(filenames[0] + " " + error);
			return false;
		}
	} else
		return true;
	
	
	// ENTRANT 1
	if (filenames.size() >= 2)
	{
		compilerForEntrant1 = new Compiler;
		compiledEntrants.push_back(compilerForEntrant1);
		
		// spot and set error
		if (!compilerForEntrant1->compile(filenames[1], error))
		{
			setError(filenames[1] + " " + error);
			return false;
		}
	} else
		return true;
	
		
	// ENTRANT 2
	if (filenames.size() >= 3)
	{
		compilerForEntrant2 = new Compiler;
		compiledEntrants.push_back(compilerForEntrant2);
		
		// spot and set error
		if (!compilerForEntrant2->compile(filenames[2], error))
		{
			setError(filenames[2] + " " + error);
			return false;
		}
	} else
		return true;
	
		
	// ENTRANT 3 
	if (filenames.size() == 4)
	{
		compilerForEntrant3 = new Compiler;
		compiledEntrants.push_back(compilerForEntrant3);
		
		// spot and set error
		if (!compilerForEntrant3->compile(filenames[3], error))
		{
			setError(filenames[3] + " " + error);
			return false;
		}
	} else
		return true;
	
	// all compiled successfully
	return true;

}

// LOAD FIELD INTO CONTAINER
bool StudentWorld::loadField()
{
	  Field f;
	  string fieldFile = getFieldFilename();
	  string error;

	 // error in loading file
	 if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	 {
		setError(fieldFile + " " + error);
		return false; // something bad happened!
	 }
	 
	 // filling container
	 for (int i = 0; i < VIEW_HEIGHT * VIEW_WIDTH; i++)
	 {
		 // translating int to x and y values
		 int x = i/VIEW_WIDTH;
		 int y = i%VIEW_WIDTH;

		 // storing item at (x, y)
		 Field::FieldItem item = f.getContentsOf(y, x);

		 ///////////////////////// ANIMATE //////////////////////////////
		 
		 // found a grasshopper
		 if (item == Field::FieldItem::grasshopper)
		 {
			 virtualWorld[i].push_back(new BabyGrasshopper(this, x, y));
		 }

		 //////////////////////// INANIMATE /////////////////////////////
		 
		 // found a rock
		 if (item == Field::FieldItem::rock)
		 {
			 virtualWorld[i].push_back(new Pebble(this, x, y));
		 }

		 // found water
		 if (item == Field::FieldItem::water)
		 {
			 virtualWorld[i].push_back(new Water(this, x, y));
		 }
		 
		 // found poison
		 if (item == Field::FieldItem::poison)
		 {
			 virtualWorld[i].push_back(new Poison(this, x, y));
		 }
		 
		 
		 	 	 	 	    // ** DECAYABLE ** //
		
		 // found food
		 if (item == Field::FieldItem::food)
		 {
			 virtualWorld[i].push_back(new Food(this, x, y));
		 }
		 
		 // found anthill 0
		 if (item == Field::FieldItem::anthill0 && compiledEntrants.size() >= 1)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 0, compiledEntrants[0]));
			 Coord c;
			 c.x = x;
			 c.y = y;
			 colonyLocations.push_back(c); 
		 }

		 // found anthill 1
		 if (item == Field::FieldItem::anthill1 && compiledEntrants.size() >= 2)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 1, compiledEntrants[1]));
			 Coord c;
			 c.x = x;
			 c.y = y;
			 colonyLocations.push_back(c);
		 }

		 // found anthill 2
		 if (item == Field::FieldItem::anthill2 && compiledEntrants.size() >= 3)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 2, compiledEntrants[2]));
			 Coord c;
			 c.x = x;
			 c.y = y;
			 colonyLocations.push_back(c);
		 }
		 
		 // found anthill 3
		 if (item == Field::FieldItem::anthill3 && compiledEntrants.size() == 4)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 3, compiledEntrants[3]));
			 Coord c;
			 c.x = x;
			 c.y = y;
			 colonyLocations.push_back(c);
		 }
		 
	  }
	
	  return true;
}

int StudentWorld::getNumAntsInColony(int colony)
{
	return antCount[colony];
}


// UPDATE DISPLAY TEXT
#include <sstream>
void StudentWorld::updateDisplayText()
{
	// TODO: finish this, page 23 in spec
	string text = "Ticks: ";
	
	int size = getFilenamesOfAntPrograms().size();
	
	ostringstream oss;
	oss << text << currTicks << " - " ;
	
	int ants0, ants1, ants2, ants3;
	//int winningAntNumber;
	
	if (size >= 1)
	{
		ants0 = getNumAntsInColony(0);
		oss << getFilenamesOfAntPrograms()[0] << ": " << ants0 << "  ";
	}
	if (size >= 2)
	{
		ants1 = getNumAntsInColony(1);
		oss << getFilenamesOfAntPrograms()[1] << ": " << ants1 << "  ";
	}
	if (size >= 3)
	{
		ants2 = getNumAntsInColony(2);
		oss << getFilenamesOfAntPrograms()[2] << ": " << ants2 << "  ";
	}
	if (size >= 4)
	{
		ants3 = getNumAntsInColony(3);
		oss << getFilenamesOfAntPrograms()[0] << ": " << ants3 << "  ";
	}
	
	setGameStatText(oss.str());
}

int StudentWorld::getWinner() 
{
	int max = -1;
	int winner = -1;
	for (int i = 0; i < antCount.size(); i++)
	{
		if (antCount[i] > max)
		{
			winner = i;
			max = antCount[i];
		}
	}
}

///////////////////////////////////////////////////////////////
////////////////////// STATUS AT (X, Y) ///////////////////////
///////////////////////////////////////////////////////////////

// CHECK IF ACTOR IS BLOCKED AT (x, y)
bool StudentWorld::isBlocked(int x, int y)
{
	// convert x, y
	int id = findID(x, y); 

	// defining iterator at id
	list<Actor*>::const_iterator it;

	// looking for pebble at id
	for (it = virtualWorld[id].begin(); it != virtualWorld[id].end(); it++)
	{
		if ((*it)->isAnimate() == false)
		{
			InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
			
			// found a blocker!
			if (iap->isBlocker())
			{
				return true;
			}
		}
	}

	// no blocker found
	return false;
}

// CHECK IF ACTOR HAS ENEMY AT (x, y)
bool StudentWorld::hasEnemy(int x, int y, int colony)
{
	// convert x, y
	int id = findID(x, y);

	// defining iterator at (x, y)'s list of Actor*
	list<Actor*>::const_iterator it;

	 // searching for food
	 it = virtualWorld[id].begin();
	 while(it != virtualWorld[id].end())
	 {		 
		 // enemies are animate
		 if ((*it)->isAnimate() == true)
		 {
			AnimateActor* aap = dynamic_cast<AnimateActor*>(*it);
			// grasshoppers do not have colonies
			if (aap->isColonized() == false)
			{
				return true; //found an enemy 
			} else
			{
				Ant* ap = dynamic_cast<Ant*>(aap);
				if (ap->getColony() != colony) // ant from another colony
				{
					return true; // found an enemy
				} // otherwise keep searching
			}
		 } 
		 it++;
	 }
	 // no enemy found
	 return false;
}

// RETURNS FOOD PTR IF THERE IS ONE
Actor* StudentWorld::hasFood(int x, int y)
{
	// convert x, y
	int id = findID(x, y);

	// defining iterator at (x, y)'s list of Actor*
	list<Actor*>::const_iterator it;

	 // searching for food
	 it = virtualWorld[id].begin();
	 while(it != virtualWorld[id].end())
	 {		 
		 // food is inanimate
		 if ((*it)->isAnimate() == false)
		 {
			InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
			// food is decayable
			if (iap->canDecay() == true)
			{
				DecayableActor* dp = dynamic_cast<DecayableActor*>(iap);
				// food is edible - found food!
				if (dp->isEdible() == true)
				{
					Food* fp = dynamic_cast<Food*>(dp);
					// return food points found
					return fp;	
				} 
			}
		 } 
		 it++;
	 }
	 // no food object found
	 return nullptr;
}

// CHECKS IF THERE IS A PHEROMONE
Actor* StudentWorld::hasPheromone(int x, int y, int colony)
{
	// convert x, y
	int id = findID(x, y);

	// defining iterator at (x, y)'s list of Actor*
	list<Actor*>::const_iterator it;

	 // searching for food
	 it = virtualWorld[id].begin();
	 while(it != virtualWorld[id].end())
	 {		 
		 // pheromones are inanimate
		 if ((*it)->isAnimate() == false)
		 {
			InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
			// pheromones are decayable
			if (iap->canDecay() == true)
			{
				DecayableActor* dp = dynamic_cast<DecayableActor*>(iap);
				// is a pheromone!
				if (dp->isPheromone() == true)
				{
					Pheromone* pp = dynamic_cast<Pheromone*>(dp);
					if (pp->getColony() == colony)
					{
						return pp; // the ant can smell this pheromone
					}
				}
			}
		 } 
		 it++;
	 }
	 // no pheromone found
	 return nullptr;
}

///////////////////////////////////////////////////////////////
//////////////// CHANGE SOME ACTOR STATUS /////////////////////
///////////////////////////////////////////////////////////////

// STUN/POISON INSECTS AT (x, y)
void StudentWorld::hurtInsects(int x, int y, char c)
{
	// convert x, y
	int id = findID(x, y); // TODO: verify this

	// defining iterator at id
	list<Actor*>::const_iterator it;
	for (it = virtualWorld[id].begin(); it != virtualWorld[id].end(); it++)
	{
		if ((*it)->isAnimate() == true)
		{
			AnimateActor* aap = dynamic_cast<AnimateActor*>(*it);

			if (c == 's') // STUN
				aap->stun();
			if (c == 'p') // POISON
				aap->poison();
		}
	}
}

// BITE RANDOM INSECT AT (x, y)
bool StudentWorld::biteRandomInsect(Actor* willBite, int x, int y, int damage)
{
	// biter is always alive
	AnimateActor* biter = dynamic_cast<AnimateActor*>(willBite);
	
	// convert x, y into ID
	int id = findID(x, y);
	int count = 0;

	list<Actor*>::const_iterator it;

	// counting all "bitable" insects
	for (it = virtualWorld[id].begin(); it != virtualWorld[id].end(); it++)
	{
		if ((*it)->isAnimate() == true)
			count++;
	}

	// no insects no biting
	if (count == 0)
		return false;

	// random insect to bite
	int address = randInt(0, count - 1);
	int index = 0;

	for (it = virtualWorld[id].begin(); it != virtualWorld[id].end(); it++)
	{
		if ((*it)->isAnimate() == true)
		{
			AnimateActor* aap = dynamic_cast<AnimateActor*>(*it);
			if (address == index)
			{
				if (aap != biter) // cannot bite itself
				{
					if (biter->isColonized() == false) //not colonized, everyone is an enemy
					{
						aap->setPoints(aap->getPoints() - damage);
					} else // biter is colonized
					{
						Ant* ant_biter = dynamic_cast<Ant*>(biter); // only ants are colonized
						if (aap->isColonized() == false) // victim not colonized, for sure an enemy
						{
							aap->setPoints(aap->getPoints() - damage);
						} else // victim colonized
						{
							Ant* ant_bitten = dynamic_cast<Ant*>(aap);
							if (ant_biter->getColony() != ant_bitten->getColony()) // not same colony
							{
								ant_bitten->setPoints(ant_bitten->getPoints() - damage);
								ant_bitten->gotBitten();
							}
						}
					}
				}
			}
			else
				index++;
		}
	}
	return true;
}

// INSECT EATS AT (x, y)
int StudentWorld::eatFood(int x, int y, int amount)
{
	Actor* aptr = hasFood(x, y);
	Food* fptr = dynamic_cast<Food*>(aptr);
	// there is no food to eat
	if (fptr == nullptr)
	{
		return 0;
	}
	
	// food points at that food object
	int foodPoints = fptr->getEnergy();
	
	// square has food!
	if (foodPoints > 0)
	{
		// if there is more than will be eaten
		if (foodPoints > amount)
		{
			fptr->setEnergy(foodPoints - amount);
			return amount;
		} else // if there is less than will be eaten
		{
			fptr->setEnergy(0);
			// ate all what was there
			return foodPoints;
		}
	} else
		return 0;
}

// GROW GH TO ADULT
void StudentWorld::growGrasshopper(Actor* bgh, int x, int y)
{
	int id = findID(x, y);

	// iterating through old id's linked list to get rid of that baby grasshopper
	list<Actor*>::iterator it;
	for (it = virtualWorld[id].begin();
		it != virtualWorld[id].end(); it++)
		{
			// found that baby grasshopper and erase it
			if ((*it) == bgh)
			{
				// new adult grasshopper
				virtualWorld[id].push_back(new AdultGrasshopper(this, x, y));
				
				// killing baby grasshopper.
				BabyGrasshopper* ptr = dynamic_cast<BabyGrasshopper*>(bgh);
				ptr->setPoints(0);
			}
		}
}

// GROW GH TO ADULT
void StudentWorld::newAntBorn(int x, int y, int colony, Compiler* c)
{
	int id = findID(x, y);

	virtualWorld[id].push_back(new Ant(this, x, y, colony, c)); // remember id changes with colony
	antCount[colony]++;
}



// REMOVE DEAD INSECTS
list<Actor*>::const_iterator StudentWorld::removeDeadActorsAndGetNext(list<Actor*>::const_iterator it, int i)
{
	bool foundFood = false;
	
	if ((*it)->isAnimate()) // animate insects to kill
	{
		AnimateActor* aap = dynamic_cast<AnimateActor*>(*it);
		 if (aap->isDead()) // Dead?
		 {
			dropFood((*it)->getX(), (*it)->getY(), 100);
			
			// delete dead actor
			delete *it;
			return virtualWorld[i].erase(it);
		 }
	} else // inanimate actors to decay
	{
		InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
		 if (iap->canDecay()) 
		 {
			 DecayableActor* dap = dynamic_cast<DecayableActor*>(iap);
			if (dap->isGone()) // Gone?
			{
				delete dap;
				return virtualWorld[i].erase(it);
			}
		 }
	}

	// hasn't removed anything, just return next
	it++;
	return it;
}

// MOVE ACTOR POINTERS
void StudentWorld::moveActor(Actor* actor, int oldX, int oldY, int newX, int newY)
{
	// storing coordinates as ID's
	int old_id = findID(oldX, oldY);
	int new_id = findID(newX, newY);

	// defining new movable
	Movable x;
	x.oldID = old_id;
	x.newID = new_id;
	x.ptr = actor;

	// adding movable to toMove vector
	toMove.push_back(x);
}


// REDIRECTING ACTOR PTS
void StudentWorld::redirectActorPtrs()
{
	// traversing movables
	for (int i = 0; i < toMove.size(); i++)
	{
		// storing current movable info
		int old_id = toMove[i].oldID;
		int new_id = toMove[i].newID;
		Actor* actor = toMove[i].ptr;

		// copying actor pointer to new location
		virtualWorld[new_id].push_back(actor);

		// iterating through old id's linked list to get rid of that actor
		list<Actor*>::iterator it;
		for (it = virtualWorld[old_id].begin();
			it != virtualWorld[old_id].end(); it++)
			{
				// found that pointer and erase it
				if ((*it) == actor)
				{
					virtualWorld[old_id].erase(it);
					break;
				}

			}
	}
}

// DROP FOOD AT X, Y
void StudentWorld::dropFood(int x, int y, int foodPts)
{
	// finding ID and setting a check of whether food was found
	int id = findID(x, y);
	
	// looking for existing food objects and if so adding
	list<Actor*>::iterator it;
	for (it = virtualWorld[id].begin();
		it != virtualWorld[id].end(); it++)
	{
		// food is inanimate
		if ((*it)->isAnimate() == false)
		{
			InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
			// food is decayable
			if (iap->canDecay() == true)
			{
				DecayableActor* dap = dynamic_cast<DecayableActor*>(iap);
				// found food
				if (dap->isEdible() == true)
				{
					dap->setEnergy(dap->getEnergy() + foodPts);
					return;
				}
			}
		}
	}
					
	 // did not find existing food
	if (it == virtualWorld[id].end())
	{
		virtualWorld[id].push_back(new Food(this, x, y, 100));
	}
}

// EMIT PHEROMONE AT X, Y
void StudentWorld::emitPheromone(int x, int y, int colony)
{
	int id = findID(x, y);
	
	// looking for existing pheromone of this colony
	Actor* ptr = hasPheromone(x, y, colony);
	Pheromone* pp = dynamic_cast<Pheromone*>(ptr);
	
	if (pp == nullptr) // no existing pheromone
	{
		virtualWorld[id].push_back(new Pheromone(this, x, y, colony));
	} else // existing pheromone
	{
		// can only go up to a strength of 768
		if ((pp->getEnergy() + 256) <= 768)
		{
			pp->setEnergy(pp->getEnergy() + 256);
		} else
		{
			pp->setEnergy(768);
		}
	}
}

///////////////////////////////////////////////////////////////
////////////////////// SIMULATION CONTROL /////////////////////
///////////////////////////////////////////////////////////////

int StudentWorld::getColonyX(int colony)
{
	if (colony >= 0 && colony < colonyLocations.size())
	{
		return colonyLocations[colony].x;
	}
}
int StudentWorld::getColonyY(int colony)
{
	if (colony >= 0 && colony < colonyLocations.size())
	{
		return colonyLocations[colony].y;
	}
}
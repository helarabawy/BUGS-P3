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
		{
			setWinner(filenames[getWinner()]);
			return GWSTATUS_PLAYER_WON;
		}
	}
	
	// continue simulation
	return GWSTATUS_CONTINUE_GAME;
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
	
	filenames = getFilenamesOfAntPrograms();
	string error;
	
	// ENTRANT 0 
	if (filenames.size() >= 1)
	{
		compilerForEntrant0 = new Compiler;
		compiledEntrants.push_back(compilerForEntrant0);
		antCount.push_back(0); // initialize ant count to 0;
	
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
		antCount.push_back(0); // initialize ant count to 0;
		
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
		antCount.push_back(0); // initialize ant count to 0;
		
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
		antCount.push_back(0); // initialize ant count to 0;
		
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

void StudentWorld::decrementAntCount(int colony)
{
	antCount[colony]--;
	updateRunningWinner();
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
		oss << filenames[0].substr(0, filenames[0].size() - 4) << ": " << ants0 << "  ";
	}
	if (size >= 2)
	{
		ants1 = getNumAntsInColony(1);
		oss << filenames[1].substr(0, filenames[1].size() - 4) << ": " << ants1 << "  ";
	}
	if (size >= 3)
	{
		ants2 = getNumAntsInColony(2);
		oss << filenames[2].substr(0, filenames[2].size() - 4) << ": " << ants2 << "  ";
	}
	if (size >= 4)
	{
		ants3 = getNumAntsInColony(3);
		oss << filenames[3].substr(0, filenames[3].size() - 4) << ": " << ants3 << "  ";
	}
	
	setGameStatText(oss.str());
}

// FUNCTION TO RETURN WINNER
int StudentWorld::getWinner()
{
	// there must be at least 6 ants to win
	if (antCount[winner] < 6)
		return -1;
	else
		return winner;
}

// FUNCTION TO REGULARLY UPDATE THE WINNER
void StudentWorld::updateRunningWinner()
{
	// traversing ant counts
	for (int i = 0; i < antCount.size(); i++)
	{
		// checking who has most ants
		if (antCount[i] > runningMax)
		{
			winner = i;
			runningMax = antCount[i];
		}
	}
	// update running max value
	runningMax = antCount[winner];
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
		 // checking if there is food
		 Food* fp = dynamic_cast<Food*>(*it);
		 if (fp != nullptr) // found food!
		 {
			return fp;
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
		Pheromone* pp = dynamic_cast<Pheromone*>(*it);
		// is a pheromone!
		if (pp != nullptr)
		{
			if (pp->getColony() == colony)
			{
				return pp; // the ant can smell this pheromone
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
	int imageID;

	// setting correct respective image id
	switch (colony)
		{
			case 0: {imageID = IID_ANT_TYPE0; break;}
			case 1: {imageID = IID_ANT_TYPE1; break;}
			case 2: {imageID = IID_ANT_TYPE2; break;}
			case 3: {imageID = IID_ANT_TYPE3; break;}
		}

	//new ant
	virtualWorld[id].push_back(new Ant(this, imageID, x, y, colony, c)); // remember id changes with colony
	antCount[colony]++;
	updateRunningWinner();
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
	Actor* ap = hasFood(x, y);
	Food* fp = dynamic_cast<Food*>(ap);

	if (fp != nullptr) // found some food object
	{
		fp->setEnergy(fp->getEnergy() + foodPts);
		return;
	} else
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
		// getting imageID
		int imageID;
		switch (colony)
			{
				case 0: {imageID = IID_PHEROMONE_TYPE0; break;}
				case 1: {imageID = IID_PHEROMONE_TYPE1; break;}
				case 2: {imageID = IID_PHEROMONE_TYPE2; break;}
				case 3: {imageID = IID_PHEROMONE_TYPE3; break;}
			}
		virtualWorld[id].push_back(new Pheromone(this, imageID, x, y, colony));
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

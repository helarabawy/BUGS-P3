#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// STARTING SIMULATION
int StudentWorld::init()
{
	currTicks = 0;

	if (!loadField())
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
	currTicks++;
	//cerr << endl << "TICKS: " <<  currTicks << endl;

	// Give each actor a chance to do something
	for(int i = 0; i < VIEW_WIDTH*VIEW_HEIGHT; i++)
	{
		 // defining iterator at (x, y)'s list of Actor*
		 list<Actor*>::const_iterator it;

		 // calling all actors at (x, y) to do sth
		 it = virtualWorld[i].begin();

		 while(it != virtualWorld[i].end())
		 {
			 (*it)->doSomething();

			 // remove dead actors
			 it = removeDeadActorsAndGetNext(it, i);
		 }
	}
	// perform all pointer redirecting
	redirectActorPtrs();
	toMove.clear();

	updateDisplayText();
	
	if (currTicks == 2000)
	{
		return GWSTATUS_NO_WINNER;
	}
	
	// the simulation is not yet over, continue!
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
			 delete *it;
			 virtualWorld[i].erase(it);

			 it = virtualWorld[i].begin();
		}
	}
}

// COMPILE ANT PROGRAMS
#include "Compiler.h"
bool StudentWorld::compileAntPrograms()
{
	vector<string> filenames = getFilenamesOfAntPrograms();
	
	for (int i = 0; i < filenames.size(); i++)
	{
		Compiler c;
		string error;
		if (c.compile(filenames[i], error))
		{
			cout << "Compiled ant named: " << c.getColonyName() << endl;
			compiled.push_back(c); // TODO: right order of ants?
		} else
			cout <<  error << endl;
	}
}


// CHECK IF ACTOR IS BLOCKED AT (x, y)
bool StudentWorld::isBlocked(int x, int y)
{
	// convert x, y
	int id = x*VIEW_WIDTH + y; // TODO: verify this

	// defining iterator at id
	list<Actor*>::const_iterator it;

	// calling all actors at (x, y) to do sth
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

// STUN INSECTS AT (x, y)
void StudentWorld::hurtInsects(int x, int y, char c)
{
	// convert x, y
	int id = x*VIEW_WIDTH + y; // TODO: verify this

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
bool StudentWorld::biteRandomInsect(int x, int y)
{
	int id = x*VIEW_WIDTH + y;
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
				aap->getBitten();
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
	// convert x, y
	int id = x*VIEW_WIDTH + y;

	// defining iterator at (x, y)'s list of Actor*
	list<Actor*>::const_iterator it;

	 // searching for food
	 it = virtualWorld[id].begin();

	 while(it != virtualWorld[id].end())
	 {
		 if ((*it)->isAnimate() == false)
		 {
			InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
			if (iap->canDecay() == true)
			{
				DecayableActor* dp = dynamic_cast<DecayableActor*>(iap);
				if (dp->isEdible() == true)
				{
					Food* fp = dynamic_cast<Food*>(dp);
					int foodPoints = fp->getPoints();
					
					if (foodPoints > amount)
					{
						fp->setPoints(foodPoints - amount);
						return amount;
					} else
					{
						fp->setPoints(0);
						return foodPoints;
					}

				} else
					return 0;
			}
		 }
		 it++;
	 }
	 return 0;
}

// GROW GH TO ADULT
void StudentWorld::growGrasshopper(Actor* bgh, int x, int y)
{
	int id = x*VIEW_WIDTH + y;

	// iterating through old id's linked list to get rid of that actor
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
				
				// looking for existing food objects
				for (it = virtualWorld[id].begin();
					it != virtualWorld[id].end(); it++)
				{
					if ((*it).isAnimate() == false)
					{
						InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
						
						if (iap->canDecay() == true)
						{
							DecayableActor* dap = dynamic_cast<DecayableActor*>(iap);
							// found food
							if (dap->isEdible() == true)
							{
								dap->setPoints(dap->getPoints() + 100);
								return;
							}
						}
					}
				}
				
				// new food object if none at this x, y
				virtualWorld[id].push_back(new Food(this, x, y, 100));
				return;
			}

		}
}

// GROW GH TO ADULT
void StudentWorld::newAntBorn(int x, int y, int colony)
{
	int id = x*VIEW_WIDTH + y;

	//virtualWorld[id].push_back(new Ant(this, x, y, colony)); // remember id changes with colony
	// TODO: increase number of ants this colony has produced

}


// REMOVE DEAD INSECTS
list<Actor*>::const_iterator StudentWorld::removeDeadActorsAndGetNext(list<Actor*>::const_iterator it, int i)
{
	if ((*it)->isAnimate())
	{
		AnimateActor* aap = dynamic_cast<AnimateActor*>(*it);
		 if (aap->isDead())
		 {
			 // body decomposes
			virtualWorld[i].push_back(new Food(this, (*it)->getX(), (*it)->getY(), 100));
			delete *it;
			return virtualWorld[i].erase(it);
		 }
	} else
	{
		InanimateActor* iap = dynamic_cast<InanimateActor*>(*it);
		 if (iap->canDecay())
		 {
			 DecayableActor* dap = dynamic_cast<DecayableActor*>(iap);
			if (dap->isGone())
			{
				delete dap;
				return virtualWorld[i].erase(it);
			}
		 }
	}

	it++;
	return it;
}

// MOVE ACTOR POINTERS
void StudentWorld::moveActor(Actor* actor, int oldX, int oldY, int newX, int newY)
{
	int old_id = oldX*VIEW_WIDTH + oldY;
	int new_id = newX*VIEW_WIDTH + newY;

	Movable x;
	x.oldID = old_id;
	x.newID = new_id;
	x.ptr = actor;

	toMove.push_back(x);
}


// REDIRECTING ACTOR PTS
void StudentWorld::redirectActorPtrs()
{
	// do this with iterator
	for (int i = 0; i < toMove.size(); i++)
	{
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
					virtualWorld[old_id].erase(it); // TODO
					break;
				}

			}
	}
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

	 // compile available ant programs
	 
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

		 // TODO: what if there aren't 4 anthills?
		 
		 // found anthill 0
		 if (item == Field::FieldItem::anthill0 && compiled.size() >= 1)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 0, compiled[0]));
		 }

		 // found anthill 1
		 if (item == Field::FieldItem::anthill1 && compiled.size() >= 2)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 1, compiled[1]));
		 }

		 // found anthill 2
		 if (item == Field::FieldItem::anthill2 && compiled.size() >= 3)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 2, compiled[2]));
		 }
		 // found anthill 3
		 if (item == Field::FieldItem::anthill3 && compiled.size() == 4)
		 {
			 virtualWorld[i].push_back(new Anthill(this, x, y, 3, compiled[3]));
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
	
/*
	int size = getFilenamesOfAntPrograms().size();
	
	int ants0, ants1, ants2, ants3;
	int winningAntNumber;
	
	ants0 = getNumAntsInColony(0);
	ants1 = getNumAntsInColony(1);
	ants2 = getNumAntsInColony(2);
	ants3 = getNumAntsInColony(3);
	
	winningAntNumber = getWinningAntNumber();
*/	
	ostringstream oss;

	oss << text << currTicks << " - ";
		//	getFilenamesOfAntPrograms()[0];

	setGameStatText(oss.str());
}


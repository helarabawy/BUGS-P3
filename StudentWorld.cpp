#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
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
	
	// Give each actor a chance to do something
	for(int i = 0; i < VIEW_WIDTH*VIEW_HEIGHT; i++)
	{
		 // translating int to x and y values
		 int x = i/VIEW_WIDTH;
		 int y = i%VIEW_WIDTH;

		 // defining iterator at (x, y)'s list of Actor*
		 list<Actor*>::const_iterator it;

		 // calling all actors at (x, y) to do sth
		 for (it = virtualWorld[i].begin(); it != virtualWorld[i].end(); it++)
		 {
			 (*it)->doSomething();

			 // remove dead actors
			 if ((*it)->isAnimate())
			 {
				 if ((*it)->isDead())
				 {
					 // body decomposes
					 virtualWorld[i].push_back(new Food(this, x, y, 100));
					 delete *it;
					 virtualWorld[i].erase(it);

					 it = virtualWorld[i].begin();
			 	 }
			 }
		 }
	}

	
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
		// found a blocker!
		if ((*it)->isBlocker())
		{
			return true;
		}
	}

	// no blocker found
	return false;
}

// STUN INSECTS AT (x, y)
void StudentWorld::stunInsects(int x, int y)
{
	// convert x, y
	int id = x*VIEW_WIDTH + y; // TODO: verify this

	// defining iterator at id
	list<Actor*>::const_iterator it;

	for (it = virtualWorld[id].begin(); it != virtualWorld[id].end(); it++)
	{
		if ((*it)->isAnimate())
			(*it)->stun();
	}
}

// POISON INSECTS AT (x, y)
void StudentWorld::poisonInsects(int x, int y)
{
	// convert x, y
	int id = x*VIEW_WIDTH + y; // TODO: verify this

	// defining iterator at id
/*	list<Actor*>::const_iterator it;

	for (it = virtualWorld[id].begin(); it != virtualWorld[id].end(); it++)
	{
		if ((*it)->isAnimate())
			(*it)->poison();
	}*/
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

		 // found a rock
		 if (item == Field::FieldItem::rock)
		 {
			 virtualWorld[i].push_back(new Pebble(this, x, y));
		 }

		 // found a grasshopper
		 if (item == Field::FieldItem::grasshopper)
		 {
			 virtualWorld[i].push_back(new BabyGrasshopper(this, x, y));
		 }

		 // found water
		 if (item == Field::FieldItem::water)
		 {
			 virtualWorld[i].push_back(new Water(this, x, y));
		 }

		 // found water
		 if (item == Field::FieldItem::food)
		 {
			 virtualWorld[i].push_back(new Food(this, x, y));
		 }

		 // found poison
		 if (item == Field::FieldItem::poison)
		 {
			 virtualWorld[IID_POISON].push_back(new Poison(this, x, y));
		 }

	  }
	
	  return true;
}


// UPDATE DISPLAY TEXT
#include <sstream>
void StudentWorld::updateDisplayText()
{
	string text = "Ticks: ";
	ostringstream oss;
	oss << text << currTicks;

	setGameStatText(oss.str());
}

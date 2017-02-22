#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;



GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

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


int StudentWorld::move()
{
	currTicks++;
	
	// Give each actor a chance to do something
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		// calling every alive/awake actor to doSomething
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			Coord c;
			c.x = i;
			c.y = j;
			Actor* q = virtualWorld[c];

			q->doSomething();

			// remove dead actors
			if (q->isDead()){
				delete virtualWorld[c];
				virtualWorld[c].erase(virtualWorld[c]);
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

void StudentWorld::cleanUp()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{

			Coord c = {.x = i, .y = j};
			delete virtualWorld[c];
			virtualWorld.erase(virtualWorld[c]);
		}
	}
}


bool StudentWorld::isBlocked(int x, int y)
{
	map<Coord, Actor*> ::iterator itr;
	Coord c = {.x = i, .y = j};
	itr = virtualWorld.find(c);

	// this is dumb the commits kinda merged in an annoying way, ignore I will fix
	if ((*itr)->getX() == x && (*itr)->getY() == y)
		return true;

	return false;
}


bool StudentWorld::isStunned(int x, int y)
{
	map<Coord, Actor*> ::iterator itr;
	Coord c = {.x = i, .y = j};
	itr = virtualWorld.find(c);


	// this is dumb the commits kinda merged in an annoying way, ignore I will fix
	if ((*itr)->getX() == x && (*itr)->getY() == y)
		return true;

	return false;
}


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

	 for (int x = 0; x < VIEW_WIDTH; x++)
	 {
		 for (int y = 0; y < VIEW_HEIGHT; y++)
		 {
			 Field::FieldItem item = f.getContentsOf(y, x);
			 Coord c;
			 c.x = x;
			 c.y = y;

			 // found a rock
			 if (item == Field::FieldItem::rock)
			 {
				 virtualWorld[c] = new Pebble(this, x, y);
			 }

			 // found a grasshopper
			 if (item == Field::FieldItem::grasshopper)
			 {
				 virtualWorld[c] = new BabyGrasshopper(this, x, y);
			 }

			 // found water
			 if (item == Field::FieldItem::water)
			 {
				 virtualWorld[c] = new Water(this, x, y));
			 }
/*

			 // found poison
			 if (item == Field::FieldItem::poison)
			 {
				 virtualWorld[IID_POISON].push_back(new Poison(this, x, y));
			 }
*/
		 }
	  }
	
	  return true;
}

#include <sstream>

void StudentWorld::updateDisplayText()
{
	string text = "Ticks: ";
	ostringstream oss;
	oss << text << currTicks;

	setGameStatText(oss.str());
}

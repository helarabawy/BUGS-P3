#include "Actor.h"
#include "StudentWorld.h"
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
	for (int i = 0; i < NUM_ACTORS; i++)
	{
		// calling every alive/awake actor to doSomething
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			Actor* q = virtualWorld[i][j];

			q->doSomething();

			// remove dead actors
			if (q->isDead()){
				int x = q->getX();
				int y = q->getY();
				delete virtualWorld[i][j];
				virtualWorld[i].erase(virtualWorld[i].begin() + j);
				// food where actor died
				if (i != IID_FOOD)
					virtualWorld[IID_FOOD].push_back(new Food(this, x, y, 100));
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
	for (int i = 0; i < NUM_ACTORS; i++)
	{
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{

			delete virtualWorld[i][j];
			virtualWorld[i].erase(virtualWorld[i].begin() + j);
		}
	}
}


bool StudentWorld::isBlocked(int x, int y)
{
	vector<Actor*> :: iterator itr;
	itr = virtualWorld[IID_ROCK].begin();
	for (int i = 0; i < virtualWorld[IID_ROCK].size(); i++, itr++)
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			return true;
	}
	return false;
}

void StudentWorld::stunInsects(int x, int y)
{
	// how do i stun this
	vector<Actor*> :: iterator itr;
	itr = virtualWorld[IID_BABY_GRASSHOPPER].begin();
	for (int i = 0; i < virtualWorld[IID_BABY_GRASSHOPPER].size(); i++, itr++)
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			(*itr)->stun();
	}
}

bool StudentWorld::isStunned(int x, int y)
{
	vector<Actor*> :: iterator itr;
	itr = virtualWorld[IID_WATER_POOL].begin();
	for (int i = 0; i < virtualWorld[IID_WATER_POOL].size(); i++, itr++)
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			return true;
	}
	return false;
}

bool StudentWorld::hasFood(int x, int y)
{
	vector<Actor*> :: iterator itr;
	itr = virtualWorld[IID_FOOD].begin();
	for (int i = 0; i < virtualWorld[IID_FOOD].size(); i++, itr++)
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			return true;
	}
	return false;
}

// this is logically wrong
void StudentWorld::depleteFood(int x, int y, int pts)
{
	vector<Actor*> :: iterator itr;
	itr = virtualWorld[IID_BABY_GRASSHOPPER].begin();
	for (int i = 0; i < virtualWorld[IID_BABY_GRASSHOPPER].size(); i++, itr++)
	{
		if ((*itr)->getX() == x && (*itr)->getY() == y)
			(*itr)->setPoints(((*itr)->getPoints - pts));
	}

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

			 // found a rock
			 if (item == Field::FieldItem::rock)
			 {
				 virtualWorld[IID_ROCK].push_back(new Pebble(this, x, y));
			 }

			 // found a grasshopper
			 if (item == Field::FieldItem::grasshopper)
			 {
				 virtualWorld[IID_BABY_GRASSHOPPER].push_back(new BabyGrasshopper(this, x, y));
			 }

			 // found water
			 if (item == Field::FieldItem::water)
			 {
				 virtualWorld[IID_WATER_POOL].push_back(new Water(this, x, y));
			 }

			 // found food
			 if (item == Field::FieldItem::food)
			 {
				 virtualWorld[IID_FOOD].push_back(new Food(this, x, y));
			 }

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

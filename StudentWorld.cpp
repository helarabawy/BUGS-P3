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
	cerr << "init() called with " << currTicks << " ticks" << endl; // TODO: delete this
	if (!loadField())
		{
			cerr << "error loading field" << endl; // TODO: delete this
			return GWSTATUS_LEVEL_ERROR;
		}
	else
		{
			cerr << "field loaded and init sucessfully" << endl; // TODO: delete this
			return GWSTATUS_CONTINUE_GAME;
		}
}


int StudentWorld::move()
{
	currTicks++;
	
	// Give each actor a chance to do something
	for (int i = 0; i < NUM_ACTORS; i++)
	{
		// iterator to iterate current vector
		vector<Actor*> :: iterator it;
		it = virtualWorld[i].begin();

		// calling every alive/awake actor to doSomething
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			Actor* q = *it;

			// TODO: review logic here
			int oldX = q->getX(), oldY = q->getY(); // TODO: is this necessary, doesn't every actor update its pos in doSomething()?
			if (!q->isDead() || !q->isSleeping())
			{
				q->doSomething();
			}

			// remove dead actors
			if (q->isDead())
				virtualWorld[i].erase(it);
		}
	}
	
	// TODO: make this helper function
	// Update the simulation Status Line
	updateDisplayText();
	
	if (currTicks == 2000)
	{
		// TODO: no checking if there is a winner since only pebbles and grasshoppers
		return GWSTATUS_NO_WINNER;
	}
	
	// the simulation is not yet over, continue!
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	// TODO: see if this is correct, should i use iterator?
	for (int i = 0; i <= NUM_ACTORS; i++)
	{
		vector<Actor*> :: iterator it;
		it = virtualWorld[i].begin();
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			virtualWorld[i].erase(it);
		}
	}
}


bool StudentWorld::hasPebble(int x, int y)
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

bool StudentWorld::loadField()
{
   Field f;
   string fieldFile = getFieldFilename();

   string error;
   
 if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
 {
 	setError(fieldFile + " " + error);
 	return false; // something bad happened!
 }
// int x = 0;
// int y = 5;
// Field::FieldItem item = f.getContentsOf(x,y); // note it’s x,y and not y,x!!!
// if (item == Field::FieldItem::rock)
// cout << "A pebble should be placed at 0,5 in the field\n"; 
 // building virtual world from file info

	
// TODO: fix x, y confusion
   for (int x = 0; x < VIEW_WIDTH; x++)
   {
	   for (int y = 0; y < VIEW_HEIGHT; y++)
	   {
		   char ch = f.getContentsOf(y, x);

		   switch (ch)
		   {
		   	   case '*':
		   	   {
		   		   virtualWorld[IID_ROCK].push_back(new Pebble(this, y, x));
		   		   break;
		   	   }
			   case 'g':
			   {
				   virtualWorld[IID_BABY_GRASSHOPPER].push_back(new BabyGrasshopper(this, y, x));
				   break;
			   }
			   default: break;
		   }

	   }
   }
}


void StudentWorld::updateDisplayText()
{
	// refer to string streams
	// TODO: figure out how ticks must be to the right of 5 characters
	cout << "Ticks:" << currTicks;
}



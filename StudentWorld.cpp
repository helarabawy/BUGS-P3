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
		//vector<Actor*> :: iterator it;

		// calling every alive/awake actor to doSomething
		//for (it = virtualWorld[i].begin(); it != virtualWorld[i].end(); it++)
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			Actor* q = virtualWorld[i][j];


			if (i == IID_BABY_GRASSHOPPER)
			cerr << endl << "==>" << "GRASSHOPPER #" << j+1 << endl;
			//Actor* q = virtualWorld[i][j];
			// TODO: review logic here
			q->doSomething();

			// remove dead actors
			if (q->isDead()){
				cerr << "KILLING GRASSHOPPER #"  << j+1 << endl;
				delete virtualWorld[i][j];
				virtualWorld[i].erase(virtualWorld[i].begin() + j);
			}
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
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			delete virtualWorld[i][j];
		}
		virtualWorld[i].clear();
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
   cerr << "Filename: " << fieldFile << endl;
   string error;
   
 if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
 {
 	setError(fieldFile + " " + error);
 	return false; // something bad happened!
 }

 cerr << "FILE LOADED SUCCESSFULLY" << endl;
	
// TODO: fix x, y confusion
   for (int x = 0; x < VIEW_WIDTH; x++)
   {
	   for (int y = 0; y < VIEW_HEIGHT; y++)
	   {
		   Field::FieldItem item = f.getContentsOf(y, x);

		   if (item == Field::FieldItem::rock)
		   {
			   virtualWorld[IID_ROCK].push_back(new Pebble(this, x, y));
		   }

		   if (item == Field::FieldItem::grasshopper)
		   {
			   virtualWorld[IID_BABY_GRASSHOPPER].push_back(new BabyGrasshopper(this, x, y));
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



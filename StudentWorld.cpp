#include "StudentWorld.h"
#include "Field.h"
#include <string>
#include <vector>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	currTicks = 0;
	
	if (!loadField())
		return GWSTATUS_LEVEL_ERROR;
	else
		return GWSTATUS_CONTINUE_GAME;
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
	updateDisplayText();   // update the ticks/ant stats text at screen top
	
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
			delete virtualWorld[i][j];

		}
	}
}

bool StudentWorld::loadField()
{
   string fieldFileName;
   Field f;

   string fieldFile = getFieldFilename();
   bool loadedFieldFileSuccessfully = f.loadField(fieldFile); // TODO: does this even return bool?

   // load field
   if (!f.loadField(fieldFile))
	   return false;     // something bad happened!

   // TODO: make sure perimeter is all pebbles?
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

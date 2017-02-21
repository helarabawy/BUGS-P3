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
	for (int i = 0; i <= NUM_ACTORS; i++)
	{
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			Actor* q = virtualWorld[i][j];
			int oldX = q->getX(), oldY = q->getY();
			if (!q->isDead() || !q->isSleeping())
			{
				q->doSomething();
			}

			if (q->isDead())
				virtualWorld[i].erase(j);
		}
	}
	
	// TODO: make this helper function
	// Update the simulation Status Line
	//updateDisplayText();   // update the ticks/ant stats text at screen top
	
	if (currTicks == 2000)
	{
		return GWSTATUS_NO_WINNER;
	}
	
	// the simulation is not yet over, continue!*/
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i <= NUM_ACTORS; i++)
	{
		for (int j = 0; j < virtualWorld[i].size(); j++)
		{
			delete virtualWorld[i][j];
			virtualWorld[i].remove(j);
		}
	}
}

bool StudentWorld::loadField()
{
   string fieldFileName;
   Field f;

   std::string fieldFile = getFieldFilename();
   bool loadedFieldFileSuccessfully = f.loadField(fieldFile);

   if (!f.loadField(fieldFile))
	   return false;     // something bad happened!

   for (int x = 0; x < VIEW_WIDTH; x++)
   {
	   for (int y = 0; y < VIEW_HEIGHT; y++)
	   {
		   char ch = getContentsOf(x, y);

		   switch (ch)
		   {
		   	   case '*':
		   	   {
		   		   virtualWorld[IID_ROCK].push_back(new Pebble(y, x));
		   		   break;
		   	   }
			   case 'g':
			   {
				   virtualWorld[IID_BABY_GRASSHOPPER].push_back(new BabyGrasshopper(y, ));
				   break;
			   }
			   default:
				   break;
		   }

	   }
   }
}

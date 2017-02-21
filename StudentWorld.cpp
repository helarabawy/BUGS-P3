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
	
	// Initialize virtual world container
	vector<Actor*> virtualWorld[15]; // 15 types of game objects
	
	// load current field details
	if (getFieldFilename() == "")
		return GWSTATUS_LEVEL_ERROR;
	
	if (!loadField())
		return GWSTATUS_LEVEL_ERROR;
	
	// allocate and insert all objects into container
	
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{/*
	updateTickCount();  // update the current tick # in the simulation
	// The term "actors" refers to all ants, ant hills, poison, pebbles,
	// baby and adult grasshoppers, food, pools of water, etc.
	
	// Give each actor a chance to do something
	for (each actor q in the simulation)
	{
		// get the actor’s current location
		int oldX = q->getX(), oldY = q->getY();
		
		if (q is still active/alive)
		{
			// ask each actor to do something (e.g. move)
			q->doSomething();
		}
		
		if (q has moved from its old square to a new square)
			updateTheDataStructureThatTracksWhereEachActorIs(q,oldX,oldY);
	}
	
	// Remove newly-dead actors after each tick
	removeDeadSimulationObjects(); // delete dead simulation objects
	
	// Update the simulation Status Line
	updateDisplayText();   // update the ticks/ant stats text at screen top
	
	// If the simulation’s over (ticks == 2000) then see if we have a winner
	if (theSimulationIsOver())
	{
		if (weHaveAWinningAnt())
		{
		setWinner(getWinningAntsName());
		return GWSTATUS_Ant_WON;
		}
		else
			return GWSTATUS_NO_WINNER;
	}
	
	// the simulation is not yet over, continue!*/
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::loadField()
{
	string fieldFileName;
	Field f;
	
	string fieldFile = getFieldFilename();
	bool loadedFieldFileSuccessfully = f.loadField(fieldFile);
	
	if (!loadedFieldFileSuccessfully)
		return false;
	
	// load was successful
	
	// TODO: LOAD STUFF
}


/*
As you can see, the data file contains a 64x64 grid of different characters that represent the different objects in the simulation.  Valid characters for your field data file are: 
  
‘*’ characters designate pebbles which block movement of all insects . Every field must have its perimeter consist entirely of pebbles. 
 
‘g’ characters specify the starting locations of baby grasshoppers. 
 
‘0’, ‘1’, ‘2’ and ‘3’ specify the location of the four ant colonies’ anthills. 
 
‘w’ characters specify pools of water. 
 
‘f’ characters specify piles of food. 
 
‘p’ characters specify poison. 
  
Space characters represent empty locations. 
*/





/*
#include "Field.h"   // you must include this file to use our Field class 
… 
int StudentWorld::someFunctionYouWriteToLoadTheField() 
{ 
   string fieldFileName; 
   Field f; 
 
   std::string fieldFile = getFieldFilename(); 
   bool loadedFieldFileSuccessfully = f.loadField(fieldFile); 
               if ( ! f.loadField(fieldFile) )        return false;     // something bad happened!   
   // otherwise the load was successful and you can access the 
   // contents of the field – here’s an example 
     int x = 0;    int y = 5; 
   Field::FIELDITEM item = f.getContentsOf(x,y); // note it’s x,y and not y,x!!!    if (item == Field::FIELDITEM::rock) 
       cout << "A pebble should be placed at 0,5 in the field\n";     x = 10;    y = 7; 
   item = f.getContentsOf(x, y);    if (item == FIELDITEM::anthill0)        cout << "The anthill for the first Ant should be at 10,7 in the field\n":     … // etc. 
} 
  
Notice that the getContentsOf() method takes the column parameter (x) first, then the row parameter (y) second.  This is different than the order one normally uses when indexing a 2dimensional array, which would be array[row][col]. Be careful! 
  
Hint: You will presumably want to use our Field class in your StudentWorld’s init() method. 

*/

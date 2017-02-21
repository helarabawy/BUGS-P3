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

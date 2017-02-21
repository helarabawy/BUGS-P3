#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{}

	virtual int init();

	virtual int move();

	virtual void cleanUp()
	{
	}

private:
	vector<Actor*> virtualWorld[NUM_ACTORS]; // 15 types of game objects
	bool loadField();
	int currTicks;
};

#endif // STUDENTWORLD_H_

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{}

	virtual ~StudentWorld() {cleanUp();}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

private:
	vector<Actor*> virtualWorld[NUM_ACTORS]; // 15 types of game objects
	bool loadField();
	void updateDisplayText();
	int currTicks;
};

#endif // STUDENTWORLD_H_

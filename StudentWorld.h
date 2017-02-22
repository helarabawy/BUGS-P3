#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

const int NUM_ACTORS = 15;
using namespace std;

class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{ }

	virtual ~StudentWorld() {cleanUp();}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	bool isBlocked(int x, int y);
	void stunInsects(int x, int y);
	bool isStunned(int x, int y);
	bool hasFood(int x, int y);
	void depleteFood(int x, int y, int pts);

private:
	vector<Actor*> virtualWorld[NUM_ACTORS]; // 15 types of game objects
	bool loadField();
	void updateDisplayText();
	int currTicks;
};

#endif // STUDENTWORLD_H_

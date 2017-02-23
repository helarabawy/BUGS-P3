#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

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

	// status at (x, y)
	bool isBlocked(int x, int y);
	/*bool isStunned(int x, int y);
	//bool hasFood(int x, int y);

	// change status at (x, y)*/
	void stunInsects(int x, int y);
	void poisonInsects(int x, int y);
	//void depleteFood(int x, int y, int pts);*/

private:

	map<int, list<Actor*>> virtualWorld; // class container

	bool loadField();
	void updateDisplayText();

	int currTicks;
};



#endif // STUDENTWORLD_H_

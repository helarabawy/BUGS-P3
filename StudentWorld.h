#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

const int NUM_ACTORS = 14;
using namespace std;

class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{ filename = assetDir; }

	virtual ~StudentWorld() {cleanUp();}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	string getFieldFilename() {return filename;}
	bool hasPebble(int x, int y);

private:
	vector<Actor*> virtualWorld[NUM_ACTORS]; // 15 types of game objects
	bool loadField();
	void updateDisplayText();
	int currTicks;
	string filename;
};

#endif // STUDENTWORLD_H_

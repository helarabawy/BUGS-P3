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
	bool isStunned(int x, int y);




private:
	struct Coord {
			int x;
			int y;
	};

	bool Coord::operator<(const Coord &a, const Coord &b) const
	{
		if (a.x < b.x)
			return true;
		return false;
	}

	map<Coord, Actor*> virtualWorld; // class container

	bool loadField();
	void updateDisplayText();

	int currTicks;
};



#endif // STUDENTWORLD_H_

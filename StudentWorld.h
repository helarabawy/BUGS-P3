#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

const int NUM_ACTORS = 15;
using namespace std;

/*
	bool operator<(const Coord &a, const Coord &b)
	{
		if (a < b)
			return true;
		return false;
	}
*/


struct Coord {
		int x;
		int y;
	};


bool operator<(const Coord &a, const Coord &b)
{
	if (a.x < b.x)
		return true;
	return false;
}



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

	bool hasPebble(int x, int y);




private:
	map<Coord, Actor*> virtualWorld; // class container


//	template <typename Data>
//	Data max(Data x, Data y)


	bool loadField();
	void updateDisplayText();

	int currTicks;
};



#endif // STUDENTWORLD_H_

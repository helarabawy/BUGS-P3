#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

// Packages/Header Files to include
#include "GameWorld.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <string>
#include <list>

using namespace std;

class Actor;

class StudentWorld : public GameWorld
{
public:

	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir) {}

	virtual ~StudentWorld() {cleanUp();}
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// status at (x, y)
	bool isBlocked(int x, int y);
	bool hasEnemy(int x, int y, int colony);
	Actor* hasFood(int x, int y); // returns pointer to food
	Actor* hasPheromone(int x, int y, int colony);

	// change status at (x, y)
	void moveActor(Actor* actor, int oldX, int oldY, int newX, int newY);
	void hurtInsects(int x, int y, char c); // TODO: make sure when ant is bitten, its gotBitten = true
	bool biteRandomInsect(Actor* biter, int x, int y, int damage);
	int eatFood(int x, int y, int amount);
	void growGrasshopper(Actor* bgh, int x, int y);
	void newAntBorn(int x, int y, int colony, Compiler* c);
	void decrementAntCount(int colony);
	void dropFood(Actor* antptr, int x, int y, int foodPts);
	void emitPheromone(int x, int y, int colony);

	// colony locations
	int getColonyX(int colony);
	int getColonyY(int colony);

	// declare winner
	int getWinner(); // todo: look at entrant requirments to win
	void updateRunningWinner();

private:
	map<int, list<Actor*>> virtualWorld; // class container
	int currTicks; // keeping track of current ticks
	int runningMax = 0;
	int winner;

	struct Movable{
		int oldID;
		int newID;
		Actor* ptr;
	};

	// compiling vector of things to move
	vector<Movable> toMove;


	// loading/updating virtualWorld
	bool compileAntPrograms();
	list<Actor*>::const_iterator removeDeadActorsAndGetNext(list<Actor*>::const_iterator it, int i);
	bool loadField();
	void updateDisplayText();
	void redirectActorPtrs();
	int findID(int x, int y) {return x*VIEW_WIDTH + y;}
	int getNumAntsInColony(int colony);

	struct Coord{
		int x;
		int y;
	};
	// storing where colonies are stationed
	vector<Coord> colonyLocations;
	vector<Compiler*> compiledEntrants; // entrants
	vector<int> antCount; // TODO: should this keep track of score
	vector<string> filenames;
};

#endif // STUDENTWORLD_H_

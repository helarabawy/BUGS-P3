

///////////////////////////////////////////////////////////////
////////////////////////// ACTOR //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor: public GraphObject {

	public:
		// Constructor
		Actor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int depth, int points = 1)
	    : GraphObject(imageID, startX, startY, dir, depth) { m_points = points; m_game = game;}

		// Destructor
		virtual ~Actor() {}

		// Public Interface
		virtual void doSomething() = 0;
		
		int getPoints() {return m_points;} // return number of points
		void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value
		bool hasPebble(int x, int y) {return m_game->hasPebble(x, y);} // TODO: fix x, y confusion
		virtual bool isDead() {return m_points <= 0;}
		virtual bool isSleeping() = 0;
		

	private:
		int m_points;
		StudentWorld* m_game;
};




///////////////////////////////////////////////////////////////
///////////////////////// PEBBLE //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef PEBBLE_H_
#define PEBBLE_H_


class Pebble: public Actor{

	public:
		// Constructor
		Pebble(StudentWorld* game, int startX, int startY): Actor(game, IID_ROCK, startX, startY, right,/*pebble doesn't move*/ 0, 1) {}
	
		// Destructor
		virtual ~Pebble() {}

		// Public Interface
		virtual void doSomething() {return;} // Pebble should do nothing during tick
		virtual bool isSleeping() {return true;} // pebble always sleeping
};

#endif // PEBBLE_H_

///////////////////////////////////////////////////////////////
//////////////////////// GRASSHOPPER //////////////////////////
///////////////////////////////////////////////////////////////

#ifndef GRASSHOPPER_H_
#define GRASSHOPPER_H_

class Grasshopper: public Actor {

	public:
		// Constructor
		Grasshopper(StudentWorld* game, int ImageID, int startX, int startY, int points)
		: ticks(0), distanceToMove(randInt(2, 10)), // member variables
		Actor(game, ImageID, startX, startY, /*random direction*/ randDir(), /*random distance*/ distanceToMove, points) {}

		// Destructor
		virtual ~Grasshopper() {}

		// Public Interface
		virtual void doSomething();
		virtual bool isSleeping() {return ticks%3 != 0;}

	private:
		// generate random direction
		Direction randDir()
		{
			int rand = randInt(1, 4);
			switch(rand)
			{
				case 1: return up;
				case 2: return right;
				case 3: return down;
				case 4: return left;
				default:return none; // TODO: check if none should be an option
			}
		}

		int ticks;
		int distanceToMove;
};

#endif // GRASSHOPPER_H_


///////////////////////////////////////////////////////////////
////////////////////// BABY GRASSHOPPER ///////////////////////
///////////////////////////////////////////////////////////////

#ifndef BABYGRASSHOPPER_H_
#define BABYGRASSHOPPER_H_

class BabyGrasshopper: public Grasshopper {

	public:
		// Constructor
		BabyGrasshopper(StudentWorld* game, int startX, int startY): Grasshopper(game, IID_BABY_GRASSHOPPER, startX, startY, 500) {}

		// Destructor
		virtual ~BabyGrasshopper() {}

		// Public Interface
		//virtual void doSomething(); not differentiated functionality yet
		
};

#endif // BABYGRASSHOPPER_H_


#endif // ACTOR_H_



///////////////////////////////////////////////////////////////
////////////////////////// ACTOR //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int NUM_ACTORS = 14;


class Actor: public GraphObject {

	public:
		// Constructor
		Actor(int imageID, int startX, int startY, Direction dir, int depth, int points = 0)
	    : GraphObject(imageID, startX, startY, dir, depth) { m_points = points;}

		// Destructor
		virtual ~Actor() {}

		// Public Interface
		virtual void doSomething() = 0;
		
		int getPoints() {return m_points;} // return number of points
		void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value
		virtual bool isDead() {return m_points == 0;}
		virtual bool isSleeping() = 0;
		
	private:
		int m_points;
};

#endif // ACTOR_H_



///////////////////////////////////////////////////////////////
///////////////////////// PEBBLE //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef PEBBLE_H_
#define PEBBLE_H_


class Pebble: public Actor{

	public:
		// Constructor
		Pebble(int startX, int startY): Actor(IID_ROCK, startX, startY, right, 1) {}
	
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
		Grasshopper(int ImageID, int startX, int startY)
		: Actor(ImageID, startX, startY, /*random direction*/ randDir(), /*random distance*/ distance, points) {}

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

		int points = 500;
		int distance = randInt(2, 10);
		int ticks = 0;
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
		BabyGrasshopper(int startX, int startY): Grasshopper(IID_BABY_GRASSHOPPER, startX, startY) {}

		// Destructor
		virtual ~BabyGrasshopper();

		// Public Interface
		virtual void doSomething();
		
};

#endif // BABYGRASSHOPPER_H_

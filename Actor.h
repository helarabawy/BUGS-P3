//////////////////// CLASS DECLARATIONS ///////////////////////


///////////////////////////////////////////////////////////////
////////////////////////// ACTOR //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor: public GraphObject {

	public:
		// Constructor
		Actor(int imageID, int startX, int startY, Direction dir, int depth, int points = 0, bool status = true)
	    : GraphObject(imageID, startX, startY, dir, depth) { m_points = points; m_status = status;}

		// Destructor
		virtual ~Actor() {}

		// Public Interface
		virtual void doSomething() = 0;
		
		int getPoints() {return m_points;} // return number of points
		void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value
		
	private:
		
		int m_points;
		bool m_status;
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
		: Actor(ImageID, startX, startY, /*random direction*/Direction.randInt(1,4), /*random distance*/randInt(2,10), 500, true) {}

		// Destructor
		virtual ~Grasshopper();

		// Public Interface
		virtual void doSomething();
		
	private:
		int m_pts = 500;
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


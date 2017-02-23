///////////////////////////////////////////////////////////////
//////////////////////// ~ ACTOR ~ ////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Abstract Actor class
class Actor: public GraphObject {

	public:
		// Constructor
		Actor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int depth)
	    : GraphObject(imageID, startX, startY, dir, depth) { m_game = game;}

		// Destructor
		virtual ~Actor() {delete m_game;}

		// Public Interface
		virtual void doSomething() = 0;
		
		virtual bool isAnimate() = 0;
		virtual bool isDead() = 0;
		virtual bool isBlocker() = 0;
		virtual void stun() = 0;

	private:
		StudentWorld* m_game;
};


///////////////////////////////////////////////////////////////
////////////////// ~ INANIMATE ACTOR ~ ////////////////////////
///////////////////////////////////////////////////////////////

#ifndef INANIMATEACTOR_H_
#define INANIMATEACTOR_H_

class InanimateActor: public Actor{

	public:
		// Constructor
		InanimateActor(StudentWorld* game, int imageID, int startX, int startY, int depth): Actor(game, imageID, startX, startY, right, depth)
		{m_game = game;}

		// Destructor
		virtual ~InanimateActor() {delete m_game;}

		// Public Interface
		virtual void doSomething() = 0;
		virtual bool isAnimate() {return false;}
		virtual bool isDead() {return true;}
		virtual void stun() {return;} // do nothing, can't be stunned


	private:
		StudentWorld* m_game;
};

#endif // INANIMATEACTOR_H_

///////////////////////////////////////////////////////////////
///////////////////////// PEBBLE //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef PEBBLE_H_
#define PEBBLE_H_

class Pebble: public InanimateActor{

	public:
		// Constructor
		Pebble(StudentWorld* game, int startX, int startY): InanimateActor(game, IID_ROCK, startX, startY, 1) {}
	
		// Destructor
		virtual ~Pebble() {}

		// Public Interface
		virtual void doSomething() {return;} // Pebble should do nothing during tick
		virtual bool isBlocker() {return true;}
};

#endif // PEBBLE_H_

///////////////////////////////////////////////////////////////
////////////////////////// WATER //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef WATER_H_
#define WATER_H_

class Water: public InanimateActor{

	public:
		// Constructor
		Water(StudentWorld* game, int startX, int startY): InanimateActor(game, IID_WATER_POOL, startX, startY, 2)
		{m_game = game;}

		// Destructor
		virtual ~Water() {delete m_game;}

		// Public Interface
		virtual void doSomething() {m_game->stunInsects(getX(), getY());}
		virtual bool isBlocker() {return false;}

	private:
		StudentWorld* m_game;
};

#endif // PEBBLE_H_


///////////////////////////////////////////////////////////////
//////////////////// ~ ANIMATE ACTOR ~ ////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ANIMATEACTOR_H_
#define ANIMATEACTOR_H_

class AnimateActor : public Actor{

	public:
		// Constructor
		AnimateActor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int depth, int healthPts): Actor(game, imageID, startX, startY, dir, depth)
		{m_game = game; m_points = healthPts;}

		// Destructor
		virtual ~AnimateActor() {delete m_game;}

		// Public Interface
		virtual bool isAnimate() {return true;}

		// dealing with points
		int getPoints() {return m_points;} // return number of points
		void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value

		// status
		virtual bool isDead() {return m_points <= 0;}
		virtual bool isSleeping() {return true;}

		// blocking
		bool isBlocked(int x, int y) {return m_game->isBlocked(x, y);}
		virtual bool isBlocker() {return false;}

		// stunning
		virtual void stun() {stunned = true;}
		void unstun() {stunned = false;}
		bool checkStunStatus() {return stunned;}

	private:

		bool stunned = false;
		StudentWorld* m_game;
		int m_points;

};

#endif // ANIMATEACTOR_H_


///////////////////////////////////////////////////////////////
//////////////////////// GRASSHOPPER //////////////////////////
///////////////////////////////////////////////////////////////

#ifndef GRASSHOPPER_H_
#define GRASSHOPPER_H_

class Grasshopper: public AnimateActor {

	public:
		// Constructor
		Grasshopper(StudentWorld* game, int ImageID, int startX, int startY, int points)
		: ticks(0), distanceToMove(randInt(2, 10)), // member variables
		AnimateActor(game, ImageID, startX, startY, /*random direction*/ randDir(), /*random distance*/ distanceToMove, points) {}

		// Destructor
		virtual ~Grasshopper() {}

		// Public Interface
		virtual void doSomething();

		virtual bool isSleeping() {return ticks%3 != 0;}
		virtual bool isStunned() = 0;
		virtual void decrementStunnedTicks() = 0;

	private:
		// generate random direction
		GraphObject::Direction randDir();

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
		BabyGrasshopper(StudentWorld* game, int startX, int startY): Grasshopper(game, IID_BABY_GRASSHOPPER, startX, startY, 500)
		{ m_game = game;}

		// Destructor
		virtual ~BabyGrasshopper() {delete m_game;}

		// Public Interface
		//virtual void doSomething(); not differentiated functionality yet


		virtual bool isStunned();
		virtual void decrementStunnedTicks();

	private:
		int stunnedTicks = 0;
		StudentWorld* m_game;
		
};

#endif // BABYGRASSHOPPER_H_



///////////////////////////////////////////////////////////////
///////////////////// ADULT GRASSHOPPER ///////////////////////
///////////////////////////////////////////////////////////////

#ifndef ADULTGRASSHOPPER_H_
#define ADULTGRASSHOPPER_H_

class AdultGrasshopper: public Grasshopper {

	public:
		// Constructor
		AdultGrasshopper(StudentWorld* game, int startX, int startY): Grasshopper(game, IID_ADULT_GRASSHOPPER, startX, startY, 500)
		{ m_game = game;}

		// Destructor
		virtual ~AdultGrasshopper() {delete m_game;}

		// Public Interface
		//virtual void doSomething(); not differentiated functionality yet

/*
		virtual bool isStunned();
		virtual void decrementStunnedTicks()
		{
			if (stunnedTicks == 1)
				stunnedTicks -= 2;
			else stunnedTicks--;
		}
*/

	private:
//		int stunnedTicks = 0;
		StudentWorld* m_game;

};

#endif // ADULTGRASSHOPPER_H_


#endif // ACTOR_H_

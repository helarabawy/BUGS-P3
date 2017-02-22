

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
		
		// dealing with points
		int getPoints() {return m_points;} // return number of points
		void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value

		// roadblocks
		bool isBlocked(int x, int y) {return m_game->isBlocked(x, y);} // TODO: fix x, y confusion

		// dealing with getting stunned
		void stun() {stunned = true;}
		void unstun() {stunned = false;}
		bool checkStunStatus() {return stunned;}

		// status
		virtual bool isDead() {return m_points <= 0;}
		virtual bool isSleeping() {return true;};
		bool hasFood() {return m_game->hasFood(getX(), getY());}
		

	private:
		bool stunned = false;
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
};

#endif // PEBBLE_H_


///////////////////////////////////////////////////////////////
///////////////////////// WATER ///////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef WATER_H_
#define WATER_H_


class Water: public Actor{

	public:
		// Constructor
		Water(StudentWorld* game, int startX, int startY): Actor(game, IID_WATER_POOL, startX, startY, right,/*pool doesn't move*/ 2, 1)
		{m_game = game;}

		// Destructor
		virtual ~Water() {}

		// Public Interface
		virtual void doSomething() {m_game->stunInsects(getX(), getY());} // Pebble should do nothing during tick


	private:
		StudentWorld* m_game;

};

#endif // WATER_H_



///////////////////////////////////////////////////////////////
////////////////////////// FOOD ///////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef FOOD_H_
#define FOOD_H_


class Food: public Actor{

	public:
		// Constructor
		Food(StudentWorld* game, int startX, int startY, int startPts = 6000): Actor(game, IID_FOOD, startX, startY, right,/*food doesn't move*/ 2, startPts)
		{m_game = game;}

		// Destructor
		virtual ~Food() {}

		// Public Interface
		virtual void doSomething() {return;} // food should do nothing during tick


	private:
		StudentWorld* m_game;

};

#endif // WATER_H_

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
		virtual bool isStunned() {return false;}
		virtual void decrementStunnedTicks() = 0;

		void eat();

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
		BabyGrasshopper(StudentWorld* game, int startX, int startY): Grasshopper(game, IID_BABY_GRASSHOPPER, startX, startY, 500)
		{ m_game = game;}

		// Destructor
		virtual ~BabyGrasshopper() {}

		// Public Interface
		//virtual void doSomething(); not differentiated functionality yet
		virtual bool isStunned();
		virtual void decrementStunnedTicks()
		{
			if (stunnedTicks == 1)
				stunnedTicks -= 2;
			else stunnedTicks--;
		}

	private:
		int stunnedTicks = 0;
		StudentWorld* m_game;
		
};

#endif // BABYGRASSHOPPER_H_


#endif // ACTOR_H_

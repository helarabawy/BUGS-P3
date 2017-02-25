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
		Actor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int depth = 0)
	    : GraphObject(imageID, startX, startY, dir, depth) { m_game = game;}

		// Destructor
		virtual ~Actor() {}

		// Public Interface
		virtual void doSomething() = 0;
		virtual bool isAnimate() = 0;
		virtual bool isDead() = 0;
		virtual bool isBlocker() = 0;
		virtual void stun() = 0;
		virtual bool checkStunStatus() = 0; // delete this
		virtual void poison() = 0;
		virtual bool canDecay() = 0;
		bool canDie() {return (isAnimate() || canDecay());}

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
		InanimateActor(StudentWorld* game, int imageID, int startX, int startY, int depth = 2): Actor(game, imageID, startX, startY, right, depth)
		{m_game = game;}

		// Destructor
		virtual ~InanimateActor() {}

		// Public Interface
		virtual void doSomething() = 0;

		// getters and setters - status
		virtual bool isAnimate() {return false;}
		virtual bool isDead() {return false;}
		virtual void stun() {return;} // do nothing, can't be stunned
		virtual bool checkStunStatus() {return false;} // delete this
		virtual void poison(){};
		virtual bool canDecay() {return false;}

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
		Water(StudentWorld* game, int startX, int startY): InanimateActor(game, IID_WATER_POOL, startX, startY)
		{m_game = game;}

		// Destructor
		virtual ~Water() {}

		// Public Interface
		virtual void doSomething() {m_game->stunInsects(getX(), getY());}
		virtual bool isBlocker() {return false;}

	private:
		StudentWorld* m_game;
};

#endif // WATER_H_



///////////////////////////////////////////////////////////////
/////////////////////////// FOOD //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef FOOD_H_
#define FOOD_H_

class Food: public InanimateActor{

	public:
		// Constructor
		Food(StudentWorld* game, int startX, int startY, int foodPts = 6000): InanimateActor(game, IID_FOOD, startX, startY)
		{m_game = game; m_foodPts = foodPts;}

		// Destructor
		virtual ~Food() {}

		// Public Interface
		virtual void doSomething() {return;}
		virtual bool canDecay() {return true;}
		virtual bool isBlocker() {return false;}
		virtual bool isDead() {return m_foodPts == 0;}


	private:
		StudentWorld* m_game;
		int m_foodPts;
};

#endif // FOOD_H_


///////////////////////////////////////////////////////////////
/////////////////////// PHERONEME /////////////////////////////
///////////////////////////////////////////////////////////////


#ifndef PHERONEME_H_
#define PHERONEME_H_

class Pheroneme: public InanimateActor{

	public:
		// Constructor
		Pheroneme(StudentWorld* game, int imageID, int startX, int startY): InanimateActor(game, imageID, startX, startY)
		{m_game = game; m_strength = 256;}

		// Destructor
		virtual ~Pheroneme() {}

		// Public Interface
		virtual void doSomething() {m_strength--;}
		virtual bool isBlocker() {return false;}
		virtual bool canDecay() {return true;}
		virtual bool isDead() {return m_strength == 0;}

	private:
		StudentWorld* m_game;
		int m_strength;
};

#endif // PHERONEME_H_



///////////////////////////////////////////////////////////////
//////////////////// ~ ANIMATE ACTOR ~ ////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ANIMATEACTOR_H_
#define ANIMATEACTOR_H_

class AnimateActor : public Actor{

	public:
		// Constructor
		AnimateActor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int healthPts): Actor(game, imageID, startX, startY, dir)
		{m_game = game; m_points = healthPts; stunned = false; poisoned = false;}

		// Destructor
		virtual ~AnimateActor() {}

		// Public Interface
		virtual void doSomething();


		virtual bool isAnimate() {return true;}
		GraphObject::Direction randDir();
		virtual bool canDecay() {return false;}

		// dealing with points
		int getPoints() {return m_points;} // return number of points
		void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value

		// status
		virtual bool isDead() {return m_points <= 0;}
		virtual bool isSleeping() = 0;

		// blocking
		bool isBlocked(int x, int y) {return m_game->isBlocked(x, y);}
		virtual bool isBlocker() {return false;}

		// stunning
		virtual void stun() {stunned = true;}
		void unstun() {stunned = false;}
		virtual bool checkStunStatus() {return stunned;}

		// poisoning
		virtual void poison() {/*m_points -= 150; poisoned = true;*/}

	private:
		bool stunned;
		bool poisoned;
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
		: distanceToMove(randInt(2, 10)), // member variables
		AnimateActor(game, ImageID, startX, startY, /*random direction*/ randDir(), points) {m_game = game;}

		// Destructor
		virtual ~Grasshopper() {}

		// Public Interface
		virtual void doSomething();

		// stunning
		virtual bool isSleeping();

		void moveStep(Direction dir, int oldX, int oldY);
		void eat();
		void virtual doFunction() = 0;

	private:
		// generate random direction
		int ticksToSleep = 0;
		bool recoveringFromStun = false;
		int distanceToMove;
		StudentWorld* m_game;
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

		void grow();
		void virtual doFunction();

	private:
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
		AdultGrasshopper(StudentWorld* game, int startX, int startY): Grasshopper(game, IID_ADULT_GRASSHOPPER, startX, startY, 1600)
		{ m_game = game;}

		// Destructor
		virtual ~AdultGrasshopper() {}

		virtual void poison() {return;}

		bool virtual checkStunStatus() {return false;} // never gets stunned
		// Public Interface
		//virtual void doSomething(); not differentiated functionality yet

		void virtual doFunction();

	private:
		void bite();
		void jump();
		void jumpTo(int x, int y);

		struct Coord{
			int x;
			int y;
		};

		vector<Coord> jumpOptions;

		StudentWorld* m_game;

};

#endif // ADULTGRASSHOPPER_H_


///////////////////////////////////////////////////////////////
/////////////////////////////// ANT ///////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ANT_H_
#define ANT_H_

class Ant: public AnimateActor {

	public:
		// Constructor
		Ant(StudentWorld* game, int imageID, int startX, int startY): AnimateActor(game, imageID, startX, startY, randDir(), 1500)
		{ m_game = game; unstun();}

		// Destructor
		virtual ~Ant() {}

		virtual void getColony() {return;}
		// Public Interface
		//virtual void doSomething(); not differentiated functionality yet

		virtual bool isSleeping();

		void virtual doFunction(){}

	private:
		int ticksToSleep = 0;
		StudentWorld* m_game;

};

#endif // ANT_H_


#endif // ACTOR_H_

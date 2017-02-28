// TODO: should i say virtual first or type first

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
	    : GraphObject(imageID, startX, startY, dir, depth) {}

		// Destructor
		virtual ~Actor() {}

		// Public Interface
		virtual void doSomething() = 0;
		virtual bool isAnimate() = 0;
		virtual bool canDie() = 0;
};

///////////////////////////////////////////////////////////////
////////////////// ~ INANIMATE ACTOR ~ ////////////////////////
///////////////////////////////////////////////////////////////

#ifndef INANIMATEACTOR_H_
#define INANIMATEACTOR_H_

class InanimateActor: public Actor{

	public:
		// Constructor
		InanimateActor(StudentWorld* game, int imageID, int startX, int startY, int depth = 2)
		: Actor(game, imageID, startX, startY, right, depth)

		// Destructor
		virtual ~InanimateActor() {}

		// Public Interface
		virtual void doSomething() = 0;

		// Getting inanimate actor status
		virtual bool canDie() {return canDecay();} //TODO: make sure overriden correctly
		virtual bool isAnimate() {return false;}
		virtual bool canDecay() {return false;}
		virtual bool isBlocker() {return false;};
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
		Pebble(StudentWorld* game, int startX, int startY)
		: InanimateActor(game, IID_ROCK, startX, startY, 1) {}
	
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
		Water(StudentWorld* game, int startX, int startY)
		: InanimateActor(game, IID_WATER_POOL, startX, startY)
		{m_game = game;}

		// Destructor
		virtual ~Water() {}

		// Public Interface
		virtual void doSomething() {m_game->hurtInsects(getX(), getY(), 's');}

	private:
		StudentWorld* m_game;
};

#endif // WATER_H_

///////////////////////////////////////////////////////////////
///////////////////////// POISON //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef POISON_H_
#define POSION

class Poison: public InanimateActor{

	public:
		// Constructor
		Poison(StudentWorld* game, int startX, int startY)
		: InanimateActor(game, IID_POISON, startX, startY, 2)
		{m_game = game;}

		// Destructor
		virtual ~Poison() {}

		// Public Interface
		virtual void doSomething() {m_game->hurtInsects(getX(), getY(),'p');} 

	private:
		StudentWorld* m_game;
};

#endif // PEBBLE_H_

///////////////////////////////////////////////////////////////
/////////////// ~ DECAYABLE INANIMATE ACTOR ~ /////////////////
///////////////////////////////////////////////////////////////

#ifndef DECAYABLEACTOR_H_
#define DECAYABLEACTOR_H_

class DecayableActor: public InanimateActor{

	public:
		// Constructor
		DecayableActor(StudentWorld* game, int imageID, int startX, int startY, int points)
		: InanimateActor(game, imageID, startX, startY)
		{m_game = game; m_points = points;}

		// Destructor
		virtual ~DecayableActor() {}

		// Public Interface
		virtual void doSomething() {setPoints(getPoints() - 1);};

		// getters and setters - status
		virtual bool canDecay() {return true;}
		virtual bool isEdible() {return false;}
		int getPoints() {return m_points;} // change from points to energy to differentiate it from alive actors
		int setPoints(int modifiedPoints) {m_points = modifiedPoints;}
		bool isGone() {return getPoints() == 0;}

	private:
		int m_points;
		StudentWorld* m_game;
};

#endif // DECAYABLEACTOR_H_

///////////////////////////////////////////////////////////////
/////////////////////////// FOOD //////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef FOOD_H_
#define FOOD_H_

class Food: public DecayableActor{

	public:
		// Constructor
		Food(StudentWorld* game, int startX, int startY, int foodPts = 6000)
		: DecayableActor(game, IID_FOOD, startX, startY, foodPts) {}

		// Destructor
		virtual ~Food() {}

		// Public Interface
		virtual void doSomething() {return;} // do nothing
		virtual bool isEdible() {return true;}
};

#endif // FOOD_H_

///////////////////////////////////////////////////////////////
/////////////////////// PHERONEME /////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef PHERONEME_H_
#define PHERONEME_H_

class Pheroneme: public DecayableActor{

	public:
		// Constructor
		Pheroneme(StudentWorld* game, int imageID, int startX, int startY)
		: DecayableActor(game, imageID, startX, startY, 256)
		{m_game = game;}

		// Destructor
		virtual ~Pheroneme() {}

	private:
		StudentWorld* m_game;
};

#endif // PHERONEME_H_

///////////////////////////////////////////////////////////////
/////////////////////////// ANTHILL ///////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ANTHILL_H_
#define ANTHILL_H_

class Anthill: public DecayableActor {

	public:
		// Constructor
		Anthill(StudentWorld* game, int startX, int startY, int colony, Compiler* c)
		: DecayableActor(game, IID_ANT_HILL, startX, startY, 8999)
		{ m_game = game; m_colony = colony;}

		// Destructor
		virtual ~Anthill() {}

		// Public Interface
		virtual void doSomething();
		int getColony() {return m_colony;} 
		// TODO: since pheroneme, ant and anthill are all associated w their respective colony
		// TODO: should I create an abstract class that they extend called colonized to store colony?

		void virtual doFunction(); // eats appropriately
		
	private:
		int m_colony;
		StudentWorld* m_game;
		Compiler* m_c;

};

#endif // ANT_H_

///////////////////////////////////////////////////////////////
//////////////////// ~ ANIMATE ACTOR ~ ////////////////////////
///////////////////////////////////////////////////////////////

#ifndef ANIMATEACTOR_H_
#define ANIMATEACTOR_H_

class AnimateActor : public Actor{

	public:
		// Constructor
		AnimateActor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int healthPts, int depth = 0): Actor(game, imageID, startX, startY, dir, depth)
		{m_game = game; m_points = healthPts; stunned = false; poisoned = false;}

		// Destructor
		virtual ~AnimateActor() {}

		// Public Interface
		virtual void doSomething();

			// Direction
			GraphObject::Direction randDir();

			// dealing with points
			int getPoints() {return m_points;} // return number of points
			void setPoints(int modifiedPoints) { m_points = modifiedPoints;} // set points to new value

			// status
			virtual bool canDie() {return true;}
			virtual bool isAnimate() {return true;}
			virtual bool isDead() {return m_points <= 0;}
			virtual bool isSleeping() = 0;

			// moving
			bool moveStep(Direction dir, int oldX, int oldY);

			// blocking
			bool isBlocked(int x, int y) {return m_game->isBlocked(x, y);}

			// stunning
			virtual void stun() {stunned = true;}
			void unstun() {stunned = false;}
			virtual bool checkStunStatus() {return stunned;}

			// eating
			bool eat(int maxFood);

			// poisoning
			virtual void poison()

	private:
			
		bool poisoned = false;
		bool stunned;
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
		AnimateActor(game, ImageID, startX, startY, /*random direction*/ randDir(), points) 
		{m_game = game;}

		// Destructor
		virtual ~Grasshopper() {}

		// Public Interface
		virtual void doSomething();

			// stunning
			virtual bool isSleeping();
			bool virtual doFunction() = 0;

	private:
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
		BabyGrasshopper(StudentWorld* game, int startX, int startY)
		: Grasshopper(game, IID_BABY_GRASSHOPPER, startX, startY, 500)
		{ m_game = game;}

		// Destructor
		virtual ~BabyGrasshopper() {}

		bool virtual doFunction();

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

		// Public Interface
		virtual void poison() {return;} // never gets poisoned
		bool virtual checkStunStatus() {return false;} // never gets stunned
		bool virtual doFunction();

	private:
		// jumping
		void jump();
		void jumpTo(int x, int y);
		bool jumped = false;

		// storing coordinates
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
		Ant(StudentWorld* game, int startX, int startY, int colony, Compiler* compiler)
		: AnimateActor(game, getImageID(), startX, startY, randDir(), 1500, 1)
		{ m_game = game; unstun(); m_colony = colony; m_compiler = compiler;}

		// Destructor
		virtual ~Ant() {}

		int getColony() {return m_colony;}
		int getImageID();
		
		// Public Interface
		virtual bool isSleeping();

		virtual void doSomething();
		bool virtual doFunction();

		void storeFood(int amount) ;

	private:
		// direction change
		void rotateClockwise();
		void rotateCounterClockwise();
		
		int m_colony;
		int storedFood = 0;
		int ticksToSleep = 0;
		Compiler* m_compiler;
		StudentWorld* m_game;

};

#endif // ANT_H_



#endif // ACTOR_H_

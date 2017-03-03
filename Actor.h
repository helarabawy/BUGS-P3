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
		virtual bool isColonized() {return false;}
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
		: Actor(game, imageID, startX, startY, right, depth) {}

		// Destructor
		virtual ~InanimateActor() {}

		// Public Interface
		virtual void doSomething() = 0;

		// Getting inanimate actor status
		virtual bool canDie() {return canDecay();} 
		virtual bool isAnimate() {return false;} // main distinguishing characteristic
		virtual bool canDecay() {return false;} // by default they do not decay
		virtual bool isBlocker() {return false;}; // by default is not a blocker
	private:

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
		virtual bool isBlocker() {return true;} // only blocker inanimate actpr
	private:
		virtual void doSomething() {return;} // Pebble should do nothing during tick
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

	private:
		StudentWorld* m_game;
	
		virtual void doSomething() {m_game->hurtInsects(getX(), getY(), 's');} // stun insects at x, y
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
		: InanimateActor(game, IID_POISON, startX, startY)
		{m_game = game;}

		// Destructor
		virtual ~Poison() {}

	private:
		StudentWorld* m_game;
		
		virtual void doSomething() {m_game->hurtInsects(getX(), getY(),'p');} // poison insects at x, y
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
		DecayableActor(StudentWorld* game, int imageID, int startX, int startY, int energy)
		: InanimateActor(game, imageID, startX, startY)
		{m_game = game; m_energy = energy;}

		// Destructor
		virtual ~DecayableActor() {}

		// Public Interface
		virtual void doSomething() {setEnergy(getEnergy() - 1);};

		// getters and setters - status
		virtual bool canDecay() {return true;} // main characteristic of this subclass
		virtual bool isEdible() {return false;} // by default decayable actors are not edible
		
		virtual int getEnergy() {return m_energy;} // change from points to energy to differentiate it from alive actors
		virtual void setEnergy(int modifiedEnergy) {m_energy = modifiedEnergy;}
		virtual bool isGone() {return getEnergy() == 0;}

	private:
		int m_energy;
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
		virtual ~Food() {} // do nothing

		// Public Interface
		virtual bool isEdible() {return true;}
		
	private:
		virtual void doSomething() {return;} // do nothing
};

#endif // FOOD_H_

///////////////////////////////////////////////////////////////
/////////////////////// PHEROMONE /////////////////////////////
///////////////////////////////////////////////////////////////

#ifndef PHEROMONE_H_
#define PHEROMONE_H_

class Pheromone: public DecayableActor{

	public:
		// Constructor
		Pheromone(StudentWorld* game, int imageID, int startX, int startY, int colony)
		: DecayableActor(game, imageID, startX, startY, 256)
		{m_game = game; m_colony = colony; moveTo(startX, startY);}
		
		// Destructor
		virtual ~Pheromone() {}

		// Public Interface
		int getColony() {return m_colony;}
				
	private:
		StudentWorld* m_game;
		int m_colony;
};

#endif // PHEROMONE_H_

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
		{ m_game = game; m_colony = colony; m_c = c;}

		// Destructor
		virtual ~Anthill() {}

		// Public Interface
		void virtual doFunction(); 
		int getColony() {return m_colony;}
		
	private:
		StudentWorld* m_game;
		Compiler* m_c;
		int m_colony;
		
		virtual void doSomething();
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
		AnimateActor(StudentWorld* game, int imageID, int startX, int startY, Direction dir, int healthPts, int depth = 0)
		:Actor(game, imageID, startX, startY, dir, depth)
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

		// blocking
		bool isBlocked(int x, int y) {return m_game->isBlocked(x, y);}

		// stunning
		virtual void stun() {stunned = true;}
		virtual bool checkStunStatus() {return stunned;}
		void unstun() {stunned = false;}

		// eating
		bool eat(int maxFood);

		// poisoning
		virtual void poison();
		
		// moving
		virtual bool moveStep(Direction dir, int oldX, int oldY);
		virtual int getDistanceToMove() {return distanceToMove;}
		virtual void setDistanceToMove(int modifiedDistance) {distanceToMove = modifiedDistance;}
			
	private:
		int distanceToMove = randInt(2, 10);
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
		:AnimateActor(game, ImageID, startX, startY, right, points)
		{m_game = game; setDistanceToMove(randInt(2, 10)); setDirection(randDir());}

		// Destructor
		virtual ~Grasshopper() {}

		// Public Interface
		virtual void doSomething();
	
		// stunning
		virtual bool isSleeping();
		virtual bool doFunction() = 0;
			
	private:
		int ticksToSleep = 0;
		bool recoveringFromStun = false;
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

		virtual bool doFunction();

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
		AdultGrasshopper(StudentWorld* game, int startX, int startY)
		: Grasshopper(game, IID_ADULT_GRASSHOPPER, startX, startY, 1600)
		{ m_game = game;}

		// Destructor
		virtual ~AdultGrasshopper() {}

		// Public Interface
		virtual void poison() {return;} // never gets poisoned
		virtual bool checkStunStatus() {return false;} // never gets stunned
		virtual bool doFunction(); // deciding to jump or bite

	private:
		StudentWorld* m_game;

		// jumping
		bool jump();
		void jumpTo(int x, int y); 
		bool isInBounds(int x, int y); // to check if in bounds

		// storing coordinates
		struct Coord{
			int x;
			int y;
		};
		
		vector<Coord> jumpOptions;
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
		Ant(StudentWorld* game, int ImageID, int startX, int startY, int colony, Compiler* compiler)
		: AnimateActor(game, ImageID, startX, startY, right, 1500, 1)
		{ m_game = game; unstun(); m_colony = colony; m_compiler = compiler; setDirection(randDir());}

		// Destructor
		virtual ~Ant() {}
		
		// Public Interface
		virtual bool isSleeping();
		virtual int getColony() {return m_colony;}
		virtual bool isColonized() {return true;}

		void storeFood(int amount);
		
		// change bitten status
		void gotBitten() {bitten = true;}

	private:

		virtual void doSomething();
		virtual void doFunction();

		Compiler* m_compiler;
		StudentWorld* m_game;
		
		// direction change
		void rotateClockwise();
		void rotateCounterClockwise();
		
		bool bitten = false;
		
		int ic = 0; // instruction counter
		int m_colony;
		int storedFood = 0;
		int ticksToSleep = 0;

};

#endif // ANT_H_

#endif // ACTOR_H_

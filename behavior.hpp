#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

#include <vector>
#include <memory>
#include "direction.hpp"

// circ dep
class Entity;

#define GRAVITY 0.5

/* Reset and set every Behave call, so overloaded functions can use it, later could hold move information about surroundings */
struct Surroundings 
{
	Entity* underneath = NULL;
	Entity* above = NULL;
	Entity* left = NULL;
	Entity* right = NULL;
};

class Behavior
{
	public:
		Behavior(Entity* owner);
		~Behavior();
		void behave(std::vector<std::shared_ptr<Entity>> entities);
		void jump();
		/* the x direction entity wants to go, used by behavior to add x speed */
		direction xPush = NONE;
		/* When slowing down, clampZero should be true so rounds to 0 eventually */
		void addXSpeed(float n, bool clampZero = false);
		/* When slowing down, clampZero should be true so rounds to 0 eventually */
		void addYSpeed(float n, bool clampZero = false);
		int getState();
		float walkAcc = 0.2;
		float xSpeed = 0; //should be set through addXSpeed(), would be better to properly private but eh
		float ySpeed = 0; //should be set through addXSpeed(), would be better to properly private but eh 
		float maxXSpeed = 7.0;
		float maxYSpeed = 20.0;
		bool gravity = true;
		Entity* owner;
		bool grounded = false;
		/* set after every behave call, so functions overloading behave can check if after calling parent */
		Surroundings surroundings;
};

#endif

#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

#include <vector>
#include <memory>
#include "direction.hpp"

// circ dep
class Entity;

class Behavior
{
	public:
		Behavior(Entity* owner, bool pickupItems = false);
		~Behavior();
		/* overloaded functions should always call parent in normal cases */
		virtual void behave();
		void jump();
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
		bool pickupItems = false;
		float frictionGround = 0.35;
};

#endif

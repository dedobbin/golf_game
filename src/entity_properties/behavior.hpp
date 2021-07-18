#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

#include <vector>
#include <memory>
#include "../utils/direction.hpp"

// circ dep
class Entity;

enum BehaviorType {
	ENEMY_A,
	PLAYER,
};

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
		bool isGrounded();
		void setGrounded(bool _grounded = true);

		float xAcc = 0;
		float yAcc = 0;

		int getState();
		void destroy(bool animation = true);
		
		Entity* owner;

		BehaviorType type;
		
		bool pickupItems = false;
		float walkAcc = 0.2;
		float maxXSpeed = 20.0;
		float maxWalkSpeed = 5.5;
		float maxYSpeed = 20.0;
		bool gravity = true;
		float frictionGround = 0.35;

		/* state */
		float xSpeed = 0; //should be set through addXSpeed(), would be better to properly private but eh
		float ySpeed = 0; //should be set through addXSpeed(), would be better to properly private but eh 
		bool destroyed = false;
		bool justJumped = false; //When didn't touch ground after a jump

	private:
		bool grounded = false;
};

#endif

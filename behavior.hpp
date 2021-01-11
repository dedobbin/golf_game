#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

#include <vector>
#include <memory>

// circ dep
class Entity;

#define GRAVITY 0.5

class Behavior
{
	public:
		Behavior(Entity* owner);
		~Behavior();
		void behave(std::vector<std::shared_ptr<Entity>> entities);
		void jump();
		/* When slowing down, clampZero should be true so rounds to 0 eventually */
		void addXSpeed(float n, bool clampZero = false);
		/* When slowing down, clampZero should be true so rounds to 0 eventually */
		void addYSpeed(float n, bool clampZero = false);
		float walkAcc = 0.3;
		float xSpeed = 0; //should be set through addXSpeed() 
		float ySpeed = 0; //should be set through addXSpeed() 
		float maxXSpeed = 5.0;
		float maxYSpeed = 20.0;
		bool gravity = true;
		Entity* owner;
		bool grounded = false;

		int getState();
		// stores last result of getStateFlags to compare if changes happend
		int lastState = 0;

};

#endif

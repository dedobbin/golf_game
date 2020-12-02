#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

// circ dep
class Entity;

class Behavior
{
	public:
		Behavior(Entity* owner);
		~Behavior();
		void behave();
		void addXSpeed(float n);
		void addYSpeed(float n);
		float walkAcc = 0.3;
		float xSpeed = 0; //should be set through addXSpeed() 
		float ySpeed = 0; //should be set through addXSpeed() 
		float maxXSpeed = 5.0;
		float maxYSpeed = 20.0;
		bool gravity = true;
		Entity* owner;

};

#endif

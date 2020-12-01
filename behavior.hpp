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
	private:
		float xSpeed = 0;
		float ySpeed = 0;
		float maxXSpeed = 5;
		float maxYSpeed = 5;
		Entity* owner;
};

#endif

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
	private:
		Entity* owner;
};

#endif

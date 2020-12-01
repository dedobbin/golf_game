#ifndef __BEHAVIOR_HPP__
#define __BEHAVIOR_HPP__

// circ dep
class Entity;

class Behavior
{
	public:
		Behavior(Entity* owner);
	private:
		Entity* owner;
};

#endif

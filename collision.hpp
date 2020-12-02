#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

// circ dep
class Entity;

class Collision
{
	public:
		Collision(Entity* owner);
		~Collision();
		static bool checkCollision(Entity* a, Entity* b);
	private:
		Entity* owner;
};

#endif
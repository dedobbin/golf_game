#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

#include "rect.hpp"
#include "direction.hpp"

// circ dep
class Entity;

class Collision
{
	public:
		Collision(Entity* owner, bool solid = true);
		~Collision();
		/* returns intersect of collision */
		static rect checkCollision(Entity* a, Entity* b);
		bool solid;
		bool pushout(Entity* collider, direction colliderDir, rect intersect);
		rect getRect();
	private:
		Entity* owner;
};

#endif
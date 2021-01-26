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
		/* Returns intersect of collision */
		static rect checkCollision(Entity* a, Entity* b);
		bool solid;
		void pushout(Entity* collider, direction colliderDir, rect intersect);
		/* Return true if entity needs to be removed from entity list */
		bool effect(Entity* collider, direction colliderDir, rect intersect);
		/* To check if living entities and moving platforms etc can move through */
		bool isNotOrSemiSolid();
		rect getRect();
		/* Reset in Behavior::behave(), circ pointer, so don't use smrt */
		std::vector<Entity*> currentColliders;
	private:
		Entity* owner;
};

#endif
#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

#include "../utils/rect.hpp"
#include "../utils/direction.hpp"

// circ dep
class Entity;

class Collision
{
	public:
		Collision(Entity* owner, bool solid = true);
		~Collision();		
		static rect getIntersect(rect a, rect b);
		static bool intersectCollides(rect intersect);
		//when entity is not solid, and still wanne check if overlaps, use forceSolid
		bool isValidPos(rect pos, bool forceSolid = false);
		void pushout(Entity* collider, direction colliderDir, rect intersect);
		/* Return true if entity needs to be removed from entity list */
		bool effect(Entity* collider, direction colliderDir, rect intersect);
		/* To check if living entities and moving platforms etc can move through */
		bool isNotOrSemiSolid();

		bool solid = true;

		/* Reset in Behavior::behave(), circ pointer, so don't use smrt */
		std::vector<Entity*> currentColliders;
	private:
		Entity* owner;
};

#endif
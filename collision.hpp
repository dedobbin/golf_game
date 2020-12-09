#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

// circ dep
class Entity;

class Collision
{
	public:
		Collision(Entity* owner, bool solid = true);
		~Collision();
		static bool checkCollision(Entity* a, Entity* b);
		bool solid;
		void effect(Entity* collider, int intersectX, int intersectY, int intersectW, int intersectH);
	private:
		Entity* owner;
};

#endif
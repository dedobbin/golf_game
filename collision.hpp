#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

// circ dep
class Entity;

struct rect {
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};
class Collision
{
	public:
		Collision(Entity* owner, bool solid = true);
		~Collision();
		/* returns intersect of collision */
		static rect checkCollision(Entity* a, Entity* b);
		bool solid;
		void effect(Entity* collider, struct rect);
	private:
		Entity* owner;
};

#endif
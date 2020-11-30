#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "visuals.hpp"

class Entity 
{
	public:
		Entity(Sprite* sprite);
		~Entity();
		Sprite* sprite;
};

#endif
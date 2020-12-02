#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "visuals.hpp"
#include "behavior.hpp"
#include "collision.hpp"
#include <memory>

class Entity 
{
	public:
		~Entity();
		int x = 0;
		int y = 0;
		int w = 100;
		int h = 100;
		std::unique_ptr<Sprite> sprite;
		std::unique_ptr<Behavior> behavior;
		std::unique_ptr<Collision> collision;
};

#endif
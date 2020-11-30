#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "visuals.hpp"
#include <memory>

class Entity 
{
	public:
		~Entity();
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		std::unique_ptr<Sprite> sprite;
};

#endif
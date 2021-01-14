#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>

class LivingEntity : public Entity
{
	public:
		void give(Item* item);
		LivingEntity(std::string name, int x = 0, int y = 0, int w = 100, int h = 100);
		Item* heldItem = NULL;
		
};
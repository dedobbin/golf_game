#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>

class LivingEntity : public Entity
{
	public:
		LivingEntity(std::string name, int x = 0, int y = 0, int w = 100, int h = 100);
		void give(Item* item);
		void dropHeldItem();
		Item* heldItem = NULL;
		
};
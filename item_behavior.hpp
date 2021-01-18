#pragma once

#include "behavior.hpp"
#include "item.hpp"

class ItemBehavior : public Behavior
{
	public:
		ItemBehavior(Item* owner);
		void behave(std::vector<std::shared_ptr<Entity>> entities);
		void interact(std::shared_ptr<Entity> e);
};
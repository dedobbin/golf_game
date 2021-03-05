#pragma once

#include "../entity_properties/behavior.hpp"
#include "../entities/item.hpp"

class ItemBehavior : public Behavior
{
	public:
		ItemBehavior(Item* owner);
		void behave();
		void interact(Entity* e);
};
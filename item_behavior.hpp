#pragma once

#include "behavior.hpp"
#include "item.hpp"

class ItemBehavior : public Behavior
{
	public:
		ItemBehavior(Item* owner);
		void behave();
		void interact(Entity* e);
};
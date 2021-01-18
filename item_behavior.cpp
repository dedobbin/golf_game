#include "item_behavior.hpp"
#include <iostream>

ItemBehavior::ItemBehavior(Item* owner)
: Behavior(owner)
{}

void ItemBehavior::behave(std::vector<std::shared_ptr<Entity>> entities)
{
	if (!((Item*) owner)->owner){
		Behavior::behave(entities);
	};
}

void ItemBehavior::interact(std::shared_ptr<Entity> e)
{
	std::cout << "DEBUG: " << owner->name << " interacts with " << e->name << std::endl;
}
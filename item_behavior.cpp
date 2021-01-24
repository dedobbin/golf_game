#include "item_behavior.hpp"
#include <iostream>
#include "living_entity.hpp"
#include <assert.h>

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
	//std::cout << "DEBUG: " << owner->name << " interacts with " << e->name << std::endl;
	if (((Item*) owner)->itemType == CLUB){
		auto holdingEntity = ((Item*)owner)->owner;
		auto living = (LivingEntity*)holdingEntity;
		assert(living->behavior);
		if(living->behavior->grounded){
			std::cout << "DEBUG: golf mode" << std::endl;
			living->golfMode.active = true;
			living->behavior->xSpeed = 0;
			living->behavior->ySpeed = 0;
		}
	}
}
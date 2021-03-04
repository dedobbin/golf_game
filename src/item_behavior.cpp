#include "item_behavior.hpp"
#include <iostream>
#include "living_entity.hpp"
#include <assert.h>

ItemBehavior::ItemBehavior(Item* owner)
: Behavior(owner)
{}

void ItemBehavior::behave()
{
	if (!((Item*) owner)->owner){
		Behavior::behave();
	};
}

void ItemBehavior::interact(Entity* e)
{
	//std::cout << "DEBUG: " << owner->name << " interacts with " << e->name << std::endl;
	if (((Item*) owner)->itemType == CLUB && e->type == BALL){
		auto holdingEntity = ((Item*)owner)->owner;
		auto living = (LivingEntity*)holdingEntity;
		assert(living->golfMode);
		if(living->behavior->grounded){
			living->golfMode->activate(e);
		}
	}
}
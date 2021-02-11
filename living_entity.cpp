#include <iostream>
#include <assert.h>
#include "living_entity.hpp"

LivingEntity::LivingEntity(std::string name, LivingEntityType livingEntityType, int x, int y, int w, int h)
: Entity(name, LIVING, x, y, w, h), livingEntityType(livingEntityType)
{}

LivingEntity::~LivingEntity()
{
	std::cout << "DEBUG: LivingEntity deconstructor" << std::endl;
}

void LivingEntity::give(Item* item)
{
	assert(item->behavior);
	item->behavior->xSpeed = 0;
	item->behavior->ySpeed = 0;
	std::cout << "DEBUG: " << this->name << " picked up " << item->name << std::endl;
	item->owner = this;
	this->heldItem = item;
}

void LivingEntity::ignoreEffectsTick()
{
	for(auto iter = ignoreEffectsMap.begin(); iter != ignoreEffectsMap.end(); ++iter)
	{
		// auto entity = iter->first;
		// auto n = iter->second;
		std::cout << iter->second << std::endl;
	}
}
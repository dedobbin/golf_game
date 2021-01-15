#include "living_entity.hpp"
#include "direction.hpp"
#include <iostream>
#include <assert.h>

LivingEntity::LivingEntity(std::string name, int x, int y, int w, int h)
: Entity(name, LIVING, x, y, w, h)
{}

void LivingEntity::give(Item* item)
{
	assert(item->behavior);
	item->behavior->xSpeed = 0;
	item->behavior->ySpeed = 0;
	std::cout << "DEBUG: " << this->name << " picked up " << item->name << std::endl;
	item->owner = this;
	this->heldItem = item;
}
#include "living_entity.hpp"
#include <iostream>

LivingEntity::LivingEntity(std::string name, int x, int y, int w, int h)
: Entity(name, LIVING, x, y, w, h)
{}

void LivingEntity::give(Item* item)
{
	std::cout << "DEBUG: " << this->name << " picked up " << item->name << std::endl;
	item->owner = this;
	this->heldItem = item;
}

void LivingEntity::dropHeldItem()
{
	this->heldItem->owner = NULL;
	this->heldItem = NULL;
}
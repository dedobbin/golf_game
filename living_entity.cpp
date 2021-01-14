#include "living_entity.hpp"

LivingEntity::LivingEntity(std::string name, int x, int y, int w, int h)
: Entity(name, LIVING, x, y, w, h)
{}

void LivingEntity::give(Item* item)
{
	item->owner = this;
	this->heldItem = item;
}
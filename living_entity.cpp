#include "living_entity.hpp"
#include "direction.hpp"
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
	direction throwDir = this->behavior->xSpeed > 0 ? RIGHT : LEFT;
	this->heldItem->pos.x = throwDir == RIGHT ? this->pos.x + this->pos.w + 10 : this->pos.x - 10;
	this->heldItem->behavior->xSpeed = RIGHT ? 4 : -4;
	this->heldItem->behavior->ySpeed = this->behavior->ySpeed;

	this->heldItem->owner = NULL;
	this->heldItem = NULL;
}
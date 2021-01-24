#include <iostream>
#include <assert.h>
#include "living_entity.hpp"

LivingEntity::LivingEntity(std::string name, int x, int y, int w, int h)
: Entity(name, LIVING, x, y, w, h)
{}

LivingEntity::~LivingEntity()
{
	//std::cout << "DEBUG: LivingEntity deconstructor" << std::endl;
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

GolfMode::GolfMode(LivingEntity* owner)
:owner(owner)
{}

void GolfMode::activate(std::shared_ptr<Entity> ball)
{
	std::cout << "DEBUG: golf mode" << std::endl;
	assert(owner->behavior);
	active = true;
	ball = ball;
	owner->behavior->xSpeed = 0;
	owner->behavior->ySpeed = 0;
	owner->pos.x = ball->pos.x;
	owner->pos.y = ball->pos.y;
}
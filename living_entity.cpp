#include "living_entity.hpp"
#include <iostream>
#include <assert.h>

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

void LivingEntity::activateGolfMode(std::shared_ptr<Entity> ball)
{
	std::cout << "DEBUG: golf mode" << std::endl;
	golfMode.active = true;
	golfMode.ball = ball;
	behavior->xSpeed = 0;
	behavior->ySpeed = 0;
	pos.x = ball->pos.x;
	pos.y = ball->pos.y;
}
#include <iostream>
#include <assert.h>
#include "living_entity.hpp"

LivingEntity::LivingEntity(std::string name, int x, int y, int w, int h)
: Entity(name, LIVING, x, y, w, h)
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

GolfMode::GolfMode(LivingEntity* owner)
:owner(owner)
{}

void GolfMode::activate(Entity* ball)
{
	std::cout << "DEBUG: golf mode" << std::endl;
	assert(owner->behavior);
	active = true;
	_ball = ball;
	owner->behavior->xSpeed = 0;
	owner->behavior->ySpeed = 0;
	setDirection(RIGHT);
}

void GolfMode::setDirection(direction dir){
	assert(_ball);
	assert(dir == LEFT || dir == RIGHT);
	//owner->pos.y = _ball->pos.y;
	_dir = dir;
	if (dir == LEFT){
		owner->pos.x = _ball->pos.x;
	} else if (dir == RIGHT){
		owner->pos.x = _ball->pos.x - owner->pos.w + _ball->pos.w;
		
	}
}
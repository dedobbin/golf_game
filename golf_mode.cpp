#include "golf_mode.hpp"

#include "living_entity.hpp"
#include <assert.h>

GolfMode::GolfMode(LivingEntity* owner)
:owner(owner)
{}

void GolfMode::activate(Entity* ball)
{
	assert(owner->behavior);
	active = true;
	_ball = ball;
	//Unsafe for multithreading..
	owner->behavior->xSpeed = 0;
	owner->behavior->ySpeed = 0;
	state = AIMING_POWER;
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
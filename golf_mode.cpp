#include "golf_mode.hpp"

#include "living_entity.hpp"
#include <assert.h>

#define METER_CURSOR_MOVE_DELAY 5 // Meter cursor should move after this amount of ticks

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

void GolfMode::tick()
{
	if (state ==AIMING_POWER || state == AIMING_HEIGHT){
		ticksAfterLastMeterMove ++;
		if (ticksAfterLastMeterMove >= METER_CURSOR_MOVE_DELAY){
			if (state == AIMING_POWER){
				power = power + 1 >= nPoints ? 0 : power + 1;
			} else if (state == AIMING_HEIGHT){
				height = height + 1 >= nPoints ? 0 : height + 1;
			}
			ticksAfterLastMeterMove = 0;
		}
	}
}
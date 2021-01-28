#include "golf_mode.hpp"

#include "living_entity.hpp"
#include <assert.h>

#define METER_CURSOR_MOVE_DELAY 3 // Meter cursor should move after this amount of ticks

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
	state = AIMING_DIRECTION;
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
			int* level;
			if (state == AIMING_POWER){
				level = &power;
			} else if (state == AIMING_HEIGHT){
				level = &height;
			}
			if (meterCursorDirection == RIGHT){
				*level+=1;
				if (*level >= nPoints - 1){
					meterCursorDirection = LEFT;
				}
			} 
			else if (meterCursorDirection == LEFT){
				*level-=1;
				if (*level <= 0){
					meterCursorDirection = RIGHT;
				}
			}
			ticksAfterLastMeterMove = 0;
		}
	}
}
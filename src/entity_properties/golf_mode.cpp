#include "golf_mode.hpp"
#include <assert.h>
#include <iostream>
#include "../entities/living_entity.hpp"

#define METER_CURSOR_MOVE_DELAY 3 // Meter cursor should move after this amount of ticks
#define SWING_ANIMATION_LEN 10

GolfMode::GolfMode(LivingEntity* owner)
:owner(owner)
{}

void GolfMode::activate(Entity* ball)
{
	assert(owner->behavior);
	assert(ball->behavior);
	assert(ball->collision);

	ball->behavior->xSpeed = 0;
	ball->behavior->ySpeed = 0;
	
	owner->behavior->xSpeed = 0;
	owner->behavior->ySpeed = 0;
	owner->behavior->xAcc = 0;

	active = true;
	//Unsafe for multithreading..
	_ball = ball;

	setDirection(_dir); //reuse last dir

	state = AIMING_POWER;
}

void GolfMode::setDirection(direction dir)
{
	assert(dir == RIGHT || dir == LEFT);
	_dir = dir;

	if (_ball){
		if (_dir == RIGHT){
			_ball->pos.x = owner->pos.x + owner->pos.w  - _ball->pos.w;
		} else if(_dir == LEFT){
			_ball->pos.x = owner->pos.x;
		}
	}
}

void GolfMode::tick()
{
	if (state == AIMING_POWER || state == AIMING_HEIGHT){
		ticksAfterLastMeterMove ++;
		if (ticksAfterLastMeterMove >= METER_CURSOR_MOVE_DELAY){
			int* level;
			if (state == AIMING_POWER){
				level = &powerCursor;
			} else if (state == AIMING_HEIGHT){
				level = &heightCursor;
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
	} else if (state == SWINGING){
		ticksAfterStartSwinging ++;
		if (ticksAfterStartSwinging >= SWING_ANIMATION_LEN){
			shoot();
		}
	}
}

void GolfMode::reset()
{
	_ball = NULL;
	powerCursor = 0;
	heightCursor = 0;
	ticksAfterLastMeterMove = 0;
	ticksAfterStartSwinging = 0;
}

void GolfMode::shoot()
{
	assert(_ball);

	if (_dir == RIGHT){
		_ball->behavior->addXSpeed(powerCursor);
	} else if (_dir == LEFT){
		_ball->behavior->addXSpeed(-powerCursor);
	}

	_ball->behavior->addYSpeed(-heightCursor);

	((LivingEntity*)owner)->ignoreEffectsList.push_back(_ball); // so don't kill myself

	reset();
	active = false;
}
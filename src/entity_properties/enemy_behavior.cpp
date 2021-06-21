#include "enemy_behavior.hpp"
#include "iostream"

EnemyBehavior::EnemyBehavior(LivingEntity* owner)
: Behavior(owner)
{}

void EnemyBehavior::behave()
{
	Behavior::behave();

	if (!grounded){
		return;
	}

	switch(((LivingEntity*)owner)->behavior->type){
		case ENEMY_A:
			if (xPush == RIGHT){
				rect rectNextPos = {
					owner->pos.x + 1,
					owner->pos.y,
					owner->pos.w,
					owner->pos.h
				};

				if (!owner->collision->isValidPos(rectNextPos, true)){
					xPush = LEFT;
				}
			} else if (xPush == LEFT){
					rect rectNextPos = {
					owner->pos.x - 1,
					owner->pos.y,
					owner->pos.w,
					owner->pos.h
				};

				if (!owner->collision->isValidPos(rectNextPos, true)){
					xPush = RIGHT;
				}
			} else if (xPush == NONE) {
				xPush = RIGHT;
			}
		break;

		default:
		break;
	}
}
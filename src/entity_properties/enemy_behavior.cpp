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
			if (((LivingEntity*)owner)->xPush == RIGHT){
				rect rectNextPos = {
					owner->pos.x + 1,
					owner->pos.y,
					owner->pos.w,
					owner->pos.h
				};

				if (!owner->collision->isValidPos(rectNextPos, true)){
					((LivingEntity*)owner)->xPush = LEFT;
				}
			} else if (((LivingEntity*)owner)->xPush == LEFT){
					rect rectNextPos = {
					owner->pos.x - 1,
					owner->pos.y,
					owner->pos.w,
					owner->pos.h
				};

				if (!owner->collision->isValidPos(rectNextPos, true)){
					((LivingEntity*)owner)->xPush = RIGHT;
				}
			} else if (((LivingEntity*)owner)->xPush == NONE) {
				((LivingEntity*)owner)->xPush = RIGHT;
			}
		break;

		default:
		break;
	}
}
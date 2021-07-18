#include "enemy_behavior.hpp"
#include "iostream"

EnemyBehavior::EnemyBehavior(LivingEntity* owner)
: Behavior(owner)
{}

void EnemyBehavior::behave()
{
	Behavior::behave();

	if (!isGrounded()){
		return;
	}

	switch(((LivingEntity*)owner)->behavior->type){
		case ENEMY_A:
			if (xAcc > 0){
				rect rectNextPos = {
					owner->pos.x + 1,
					owner->pos.y,
					owner->pos.w,
					owner->pos.h
				};

				if (!owner->collision->isValidPos(rectNextPos, true)){
					xAcc = -walkAcc;
				}
			} else if (xAcc < LEFT){
					rect rectNextPos = {
					owner->pos.x - 1,
					owner->pos.y,
					owner->pos.w,
					owner->pos.h
				};

				if (!owner->collision->isValidPos(rectNextPos, true)){
					xAcc = walkAcc;
				}
			} else if (xAcc == 0) {
				xAcc = walkAcc;
			}
		break;

		default:
		break;
	}
}
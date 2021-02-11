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
	if ( ((LivingEntity*)owner)->livingEntityType == ENEMY_A){
		// switch (((LivingEntity*)owner)->xPush ){
		// 	case LEFT:
		// 		if (!owner->collision->isValidPos({
		// 			owner->pos.x -1,
		// 			owner->pos.y,
		// 			owner->pos.w,
		// 			owner->pos.h,
		// 		})){
		// 			((LivingEntity*)owner)->xPush = RIGHT;
		// 		}
		// 		break;
		// 	case RIGHT:
		// 		if (!owner->collision->isValidPos({
		// 			owner->pos.x + 1,
		// 			owner->pos.y,
		// 			owner->pos.w,
		// 			owner->pos.h,
		// 		})){
		// 			((LivingEntity*)owner)->xPush = LEFT;
		// 		}

		// 		break;

		// 	case NONE:
		// 		((LivingEntity*)owner)->xPush = RIGHT;
		// 		break;
		// }
	}
}
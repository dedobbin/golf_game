#include "enemy_behavior.hpp"
#include "iostream"

EnemyBehavior::EnemyBehavior(LivingEntity* owner)
: Behavior(owner)
{}


void EnemyBehavior::behave(std::vector<std::shared_ptr<Entity>> entities)
{
	Behavior::behave(entities);

	if (!grounded){
		return;
	}

	switch(xPush){
		case NONE:
			xPush = LEFT;
			break;
		case LEFT:
			if (xSpeed == 0){
				xPush = RIGHT;
			}
			break;
		case RIGHT:
			if (xSpeed == 0){
				xPush = LEFT;
			}
			break;
	}
}
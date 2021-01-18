#include "enemy_behavior.hpp"

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
			if (surroundings.left){
				if (! (!surroundings.left->collision || surroundings.left->collision->isNotOrSemiSolid())){
					xPush = RIGHT;
				}
			}
			break;
		case RIGHT:
			if (surroundings.right){
				if (! (!surroundings.right->collision || surroundings.right->collision->isNotOrSemiSolid())){
					xPush = LEFT;
				}
			}
			break;
	}
}
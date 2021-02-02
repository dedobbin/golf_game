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

	//TODO: turn around when walk into wall
	((LivingEntity*)owner)->xPush = LEFT;
}
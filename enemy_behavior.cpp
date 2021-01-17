#include "enemy_behavior.hpp"

EnemyBehavior::EnemyBehavior(LivingEntity* owner)
: Behavior(owner)
{}


void EnemyBehavior::behave(std::vector<std::shared_ptr<Entity>> entities)
{
	Behavior::behave(entities);
}
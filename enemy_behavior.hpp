#pragma once

#include "behavior.hpp"
#include "living_entity.hpp"

class EnemyBehavior : public Behavior
{
	public: 
		EnemyBehavior(LivingEntity* owner);
		void behave(std::vector<std::shared_ptr<Entity>> entities);
};
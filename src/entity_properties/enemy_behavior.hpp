#pragma once

#include "behavior.hpp"
#include "../entities/living_entity.hpp"

class EnemyBehavior : public Behavior
{
	public: 
		EnemyBehavior(LivingEntity* owner);
		void behave();
};
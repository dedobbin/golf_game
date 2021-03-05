#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>
#include "../entity_properties/golf_mode.hpp"

enum LivingEntityType {
	ENEMY_A,
	PLAYER,
};
class LivingEntity : public Entity
{
	public:
		LivingEntity(std::string name, LivingEntityType livingEntityType, int x = 0, int y = 0, int w = 100, int h = 100);
		~LivingEntity();
		void give(Item* item);
		/* the x direction entity wants to go, used by behavior to add x speed */
		direction xPush = NONE;
		/* Should be set using give, so owner of item is also set correctly, would be better to properly private but eh */
		Item* heldItem = NULL;
		std::unique_ptr<GolfMode> golfMode;
		LivingEntityType livingEntityType;
		/* stores entities should be ignored and for amount of ticks */
		std::unordered_map<Entity*, int> ignoreEffectsMap;
		void ignoreEffectsTick();
};
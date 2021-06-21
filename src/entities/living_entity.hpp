#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>
#include "../entity_properties/golf_mode.hpp"

class LivingEntity : public Entity
{
	public:
		LivingEntity(std::string name, int x = 0, int y = 0, int w = 100, int h = 100);
		~LivingEntity();
		void give(Item* item);
		/* Should be set using give, so owner of item is also set correctly, would be better to properly private but eh */
		Item* heldItem = NULL;
		std::unique_ptr<GolfMode> golfMode;
		/* stores entities should be ignored on collision. Behavior::behave() will remove any entries that don't touch entity (anymore) every tick*/
		std::vector<Entity*> ignoreEffectsList;
};
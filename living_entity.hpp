#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>

//TODO: I don't think all living entities should have golfmode.. should probably go in living_behavior
struct GolfMode 
{
	bool active = false;
	std::shared_ptr<Entity> ball;
};

class LivingEntity : public Entity
{
	public:
		LivingEntity(std::string name, int x = 0, int y = 0, int w = 100, int h = 100);
		~LivingEntity();
		void give(Item* item);
		void activateGolfMode(std::shared_ptr<Entity> ball);
		/* Should be set using give, so owner of item is also set correctly, would be better to properly private but eh */
		Item* heldItem = NULL;
		//TODO: should go in living_behavior
		GolfMode golfMode;
};
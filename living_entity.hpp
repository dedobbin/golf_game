#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>

class LivingEntity;
class GolfMode 
{
	public:
		GolfMode(LivingEntity* owner);
		void activate(std::shared_ptr<Entity> ball);
		bool active = false;
	private:
		std::shared_ptr<Entity> ball;
		Entity* owner;
};

class LivingEntity : public Entity
{
	public:
		LivingEntity(std::string name, int x = 0, int y = 0, int w = 100, int h = 100);
		~LivingEntity();
		void give(Item* item);
		/* Should be set using give, so owner of item is also set correctly, would be better to properly private but eh */
		Item* heldItem = NULL;
		std::unique_ptr<GolfMode> golfMode;
};
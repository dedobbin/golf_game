#pragma once

#include "item.hpp"
#include "entity.hpp"
#include <memory>

class LivingEntity : public Entity
{
	public:
		std::unique_ptr<Item> heldItem;

};
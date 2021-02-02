#pragma once

#include <vector>
#include <memory>
#include "entity.hpp"

class World
{
	public:
		static std::vector<std::shared_ptr<Entity>> entities;
};
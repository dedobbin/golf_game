#pragma once

#include <memory>
#include <vector>
#include "entity.hpp"

class World
{
	public:
		static std::vector<std::shared_ptr<Entity>> entities; 
};
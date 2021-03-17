#pragma once

#include <vector>
#include <memory>
#include "entities/entity.hpp"

class World
{
	public:
		static std::vector<std::shared_ptr<Entity>> entities;
		static float gravity;
		static int w;
		static int h;
		static std::string name;
};
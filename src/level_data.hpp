#pragma once

#include "entities/entity.hpp"

#include <vector>

struct LevelData
{
	std::vector<std::shared_ptr<Entity>> entities;
	float gravity;
	int w;
	int h;
	std::string name;
};
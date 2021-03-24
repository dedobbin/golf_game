#pragma once

#include "entities/entity.hpp"

#include <vector>

struct LevelData
{
	std::string name;
	std::string filename;
	int w;
	int h;
	float gravity;
	std::vector<std::shared_ptr<Entity>> entities;
};
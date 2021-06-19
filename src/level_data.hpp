#pragma once

#include "entities/entity.hpp"
#include "utils/json.hpp"

#include <vector>

struct LevelData
{
	nlohmann::json initialState;
	std::string name;
	int w;
	int h;
	float gravity;
	std::vector<std::shared_ptr<Entity>> entities;
};
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "utils/json.hpp"
#include "level_data.hpp"
namespace Import
{
	std::shared_ptr<LevelData> importLevel(nlohmann::json data, std::unordered_map<std::string, SDL_Texture*> spriteSheets);
};
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL.h>

#include "level_data.hpp"
namespace Import
{
	LevelData* importLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> spriteSheets);
};
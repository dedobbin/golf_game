#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL.h>

// circ dep
struct LevelData;

namespace Import
{
	std::unique_ptr<LevelData> importLevel(std::string filePath, std::unordered_map<std::string, SDL_Texture*> spriteSheets);
};
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL.h>

namespace Import
{
	void importLevel(std::string filePath, std::unordered_map<std::string, SDL_Texture*> spriteSheets);
};
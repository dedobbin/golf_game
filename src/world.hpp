#pragma once

#include <vector>
#include <memory>
#include "entities/entity.hpp"
#include "import_level.hpp"
#include "level_data.hpp"
class World
{
	public:
		// Filename is key, so levels with same filename will overwrite eachother, even if in different folders somehow
		static void loadLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> spritesheets);
		static std::shared_ptr<LevelData> activeLevel;

};
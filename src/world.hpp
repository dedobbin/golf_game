#pragma once

#include <vector>
#include <memory>
#include "entities/entity.hpp"
#include "import_level.hpp"

struct LevelData
{
	std::vector<std::shared_ptr<Entity>> entities;
	float gravity;
	int w;
	int h;
	std::string name;
};
class World
{
	public:
		static std::unordered_map<std::string, std::unique_ptr<LevelData>> levels;
		static std::vector<std::shared_ptr<Entity>> entities;
		static float gravity;
		static int w;
		static int h;
		static std::string name;
		static void loadLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> spritesheets);
};
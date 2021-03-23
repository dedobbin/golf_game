#include "world.hpp"

std::unordered_map<std::string, std::shared_ptr<LevelData>> World::levels = {};
std::shared_ptr<LevelData> World::activeLevel = NULL;

void World::loadLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	if (levels.find(filename) == levels.end()){
		std::string path = RESOURCES_PATH + std::string("/levels/") + filename;
		levels[filename] = std::shared_ptr<LevelData>(Import::importLevel(path, spritesheets));	
	}
}
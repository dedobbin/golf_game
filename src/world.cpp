#include "world.hpp"

std::shared_ptr<LevelData> World::activeLevel = NULL;

void World::loadLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	activeLevel = std::shared_ptr<LevelData>(Import::importLevel(filename, spritesheets));	
}
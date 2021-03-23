#include "world.hpp"

std::vector<std::shared_ptr<Entity>> World::entities = {};

float World::gravity = 0.65;
int World::w = 10000000;
int World::h = 1000;
std::string World::name = "";
std::unordered_map<std::string, std::unique_ptr<LevelData>> World::levels = {};


void World::loadLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	if (levels.find(filename) == levels.end()){
		std::string path = RESOURCES_PATH + std::string("/levels/") + filename;
		levels[filename] = std::unique_ptr<LevelData>(Import::importLevel(path, spritesheets));	
	}
	World::entities = levels[filename]->entities;
	World::gravity = levels[filename]->gravity;
	World::w = levels[filename]->w;
	World::h = levels[filename]->h;
	World::name = levels[filename]->name;
}

#pragma once

#include "entities/living_entity.hpp"
#include "entities/entity.hpp"
//#include <SDL2/SDL.h>


/* Creates entities for testing. For actual game entities should be read from external source like a .wsp file */
class EntityFactory 
{
	public:
		EntityFactory(std::unordered_map<std::string, SDL_Texture*> spritesheets);
		LivingEntity* createPlayer(int x, int y);
		Item* createGolfClub(int x, int y);
		Entity* createBall(int x, int y);
		Entity* createBlock(int x, int y, int w = 100, int h = 100);
		LivingEntity* createEnemy(int x, int y);

	private:
		// not owned, it should be destroyed here (but in Visuals)
		std::unordered_map<std::string, SDL_Texture*> spritesheets;
};
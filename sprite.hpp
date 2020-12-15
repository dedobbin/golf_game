#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL2/SDL.h>
#include "camera.hpp"
#include <map>
#include <memory>
#include "camera.hpp"
// #include <SDL2/SDL_image.h>

// circ dep
class Entity;

class Sprite 
{
	public:
		Sprite(Entity* owner);
		Sprite(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner);
		~Sprite();
		virtual void render(SDL_Renderer* renderer, Camera* camera);
		SDL_Rect getPos(Camera* camera = NULL);
		Entity* owner;

		//Not used by animated sprite child class
		SDL_Texture* spritesheet = NULL;
		SDL_Rect src = {0,0,0,0};

};

#endif
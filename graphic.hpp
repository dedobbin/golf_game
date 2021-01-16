#pragma once

#include <SDL2/SDL.h>
#include "camera.hpp"
#include <map>
#include <memory>
#include "camera.hpp"
// #include <SDL2/SDL_image.h>

// circ dep
class Entity;

struct Sprite 
{
	SDL_Texture* spritesheet = NULL;
	SDL_Rect src = {0,0,0,0};
	float angle = 0.0f;
};

class Graphic 
{
	public:
		Graphic(Entity* owner);
		Graphic(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner);
		~Graphic();
		virtual Sprite getSprite();
		virtual void render(SDL_Renderer* renderer, Camera* camera);
		SDL_Rect getPos(Camera* camera = NULL);
		Entity* owner;

		//Not used by animated sprite child class
		Sprite sprite;

};
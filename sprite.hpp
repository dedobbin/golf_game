#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL2/SDL.h>
#include "camera.hpp"
// #include <SDL2/SDL_image.h>

// circ dep
class Entity;

class Sprite 
{
	public:
		Sprite(Entity* owner);
		~Sprite();
		SDL_Rect getPos(Camera* camera = NULL);
		Entity* owner;
};

#endif
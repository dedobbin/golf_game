#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

// circ dep
class Entity;

class Sprite 
{
	public:
		Sprite(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner);
		~Sprite();
		SDL_Rect getPos();
		SDL_Texture* spritesheet;
		SDL_Rect src;
		Entity* owner;
};

#endif
#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

class Sprite 
{
	public:
		Sprite(SDL_Texture* spritesheet, SDL_Rect src);
		~Sprite();
		SDL_Texture* spritesheet;
		SDL_Rect src;
};

#endif
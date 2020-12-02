#include "sprite.hpp"
#include <iostream>

// circ dep
#include "entity.hpp"

Sprite::Sprite(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner)
: spritesheet(spritesheet), src(src), owner(owner)
{}

SDL_Rect Sprite::getPos()
{
	// TODO: base on camera etc
	return {
		owner->x % SCREEN_W,
		owner->y % SCREEN_H,
		owner->w,
		owner->h
	};
}

Sprite::~Sprite()
{
	std::cout << "DEBUG: Sprite deconstructor" << std::endl;
}
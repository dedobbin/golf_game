#include "sprite.hpp"
#include <iostream>

Sprite::Sprite(SDL_Texture* spritesheet, SDL_Rect src)
: spritesheet(spritesheet), src(src)
{}

Sprite::~Sprite()
{
	std::cout << "DEBUG: Sprite deconstructor" << std::endl;
}
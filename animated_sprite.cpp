#include "animated_sprite.hpp"
#include <iostream>

// circ dep
#include "entity.hpp"

AnimatedSprite::AnimatedSprite(Entity* owner)
:Sprite(owner)
{}

void AnimatedSprite::render(SDL_Renderer* renderer)
{
	std::cout << "DEBUG: AnimatedSprite::render()" << std::endl;
}
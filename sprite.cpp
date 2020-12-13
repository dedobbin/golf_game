#include "sprite.hpp"
#include <iostream>

// circ dep
#include "entity.hpp"

Sprite::Sprite(Entity* owner)
: owner(owner)
{}

SDL_Rect Sprite::getPos(Camera* camera)
{
	if (!camera){
		return {
			owner->x,
			owner->y,
			owner->w,
			owner->h
		};
	}
	return {
		owner->x - camera->camRect.x,
		owner->y - camera->camRect.y,
		owner->w,
		owner->h
	};
}

Sprite::~Sprite()
{
	//std::cout << "DEBUG: Sprite deconstructor" << std::endl;
}
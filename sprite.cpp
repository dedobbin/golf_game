#include "sprite.hpp"
#include <iostream>

// circ dep
#include "entity.hpp"

Sprite::Sprite(Entity* owner)
: owner(owner)
{}

Sprite::Sprite(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner)
: owner(owner), spritesheet(spritesheet), src(src)
{}

void Sprite::render(SDL_Renderer* renderer, Camera* camera)
{
	auto pos = getPos(camera);

	//if (SDL_RenderCopyEx( renderer, sprite->texture, &sprite->src, &sprite->pos , NULL, NULL, sprite->flip) < 0){
	if (SDL_RenderCopy( renderer, spritesheet, &src, &pos) < 0){
		std::cerr << "Failed to render sprite " + owner->name << std::endl;
	}
}

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
#include "graphic.hpp"
#include <iostream>

// circ dep
#include "entity.hpp"

Graphic::Graphic(Entity* owner)
: owner(owner)
{}

Graphic::Graphic(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner)
: owner(owner), sprite({spritesheet, src})
{}

Graphic::~Graphic()
{
	//std::cout << "DEBUG: Sprite deconstructor" << std::endl;
}

void Graphic::render(SDL_Renderer* renderer, Camera* camera)
{
	auto pos = getPos(camera);

	auto sprite = getSprite();

	//if (SDL_RenderCopyEx( renderer, sprite->texture, &sprite->src, &sprite->pos , NULL, NULL, sprite->flip) < 0){
	if (SDL_RenderCopy( renderer, sprite.spritesheet, &sprite.src, &pos) < 0){
		std::cerr << "Failed to render sprite " + owner->name << std::endl;
	}
}

Sprite Graphic::getSprite()
{
	return sprite;
}

SDL_Rect Graphic::getPos(Camera* camera)
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
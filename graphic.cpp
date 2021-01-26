#include "graphic.hpp"
#include <iostream>

#include "item.hpp"
#include "living_entity.hpp"

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
	SDL_Rect pos = getPos(camera);
	auto sprite = getSprite();

	float angle = 0.0f;
	SDL_RendererFlip flip = (SDL_RendererFlip)false;

	if (owner->type == ITEM && ((Item*)owner)->owner){
		if (((Item*)owner)->owner->type == LIVING){
			auto living = (LivingEntity*)((Item*)owner)->owner;

			if (living->golfMode && living->golfMode->active){
				angle = -150;
				pos.y -= living->pos.h / 2;
				pos.x += living->pos.w / 3;
				flip = (SDL_RendererFlip)true;
			} else {
				angle = -18;
				pos.x += ((Item*)owner)->owner->pos.w / 10 + 10;
				pos.y -= living->pos.w / 20;
			}
		}
	}

	if (SDL_RenderCopyEx( renderer, sprite.spritesheet, &sprite.src, &pos, angle, NULL, flip) < 0){
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
			owner->pos.x,
			owner->pos.y,
			owner->pos.w,
			owner->pos.h
		};
	}
	return {
		owner->pos.x - camera->camRect.x,
		owner->pos.y - camera->camRect.y,
		owner->pos.w,
		owner->pos.h
	};
}
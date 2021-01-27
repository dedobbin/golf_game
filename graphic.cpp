#include "graphic.hpp"
#include <iostream>
#include "living_entity.hpp"

// circ dep
#include "entity.hpp"

Animation::Animation(SDL_Texture* spritesheet)
: spritesheet(spritesheet)
{}

Frame::Frame(int x, int y, int w, int h)
: src({x, y, w, h})
{}

Frame::~Frame()
{
	//std::cout << "DEBUG: Frame deconstructor" << std::endl;
}

Animation::~Animation()
{
	//std::cout << "DEBUG: Animation deconstructor" << std::endl;
}


Graphic::Graphic(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner)
: owner(owner), isAnimated(false)
{
	auto def = new Animation(spritesheet);
	def->frames.push_back(std::make_unique<Frame>(src.x, src.y, src.w, src.h));
	animations[DEFAULT] = std::unique_ptr<Animation>(def);
}

Graphic::Graphic(Entity* owner)
: owner(owner), isAnimated(true)
{}

void Graphic::changeState(AnimationState newState)
{
	if (curAnimationState == newState) return;
	curAnimationState = newState;
}


Sprite Graphic::getSprite()
{
	auto animation = animations[curAnimationState].get();
	auto curFrame = animation->frames[animation->curFrame].get();
	return {animation->spritesheet, curFrame->src};
}

void Graphic::render(SDL_Renderer* renderer, Camera* camera)
{	
	
	if (owner->type == LIVING  && ((LivingEntity*)owner)->heldItem && ((LivingEntity*)owner)->golfMode && !((LivingEntity*)owner)->golfMode->active){
		((LivingEntity*)owner)->heldItem->graphic->render(renderer, camera);
	}

	auto sprite = getSprite();
	auto pos = getPos(camera);
	
	float angle = 0.0f;
	SDL_RendererFlip flip = (SDL_RendererFlip)false;
	
	if (owner->type == ITEM && ((Item*)owner)->owner){
		if (((Item*)owner)->owner->type == LIVING){
			auto living = (LivingEntity*)((Item*)owner)->owner;

			if (living->golfMode && living->golfMode->active){
				if (living->golfMode->_dir == RIGHT){
					angle = -150;
					pos.y -= living->pos.h / 2;
					pos.x += living->pos.w / 3;
					flip = (SDL_RendererFlip)true;
				} else if (living->golfMode->_dir == LEFT){
					angle = 150;
					pos.y -= living->pos.h / 2;
					pos.x -= living->pos.w / 2;
				}
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

	//draw in front
	if (owner->type == LIVING  && ((LivingEntity*)owner)->heldItem && ((LivingEntity*)owner)->golfMode && ((LivingEntity*)owner)->golfMode->active){
		((LivingEntity*)owner)->heldItem->graphic->render(renderer, camera);
	}

	frameTick();
}

void Graphic::frameTick()
{
	if (!isAnimated){
		return;
	}

	if (animations.find(curAnimationState) == animations.end()){
		return;
	}
	auto animation = animations[curAnimationState].get();

	if (animation->curFrameTick++ < 20 - animation->animationSpeed){
		return;
	}

	animation->curFrameTick = 0;

	if (animation->curFrame + 1 >= animation->frames.size()){
		if (animation->loop){
			animation->curFrame = 0;
			return;
		} else {
			return;
		}  
	}

	animation->curFrame ++;
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
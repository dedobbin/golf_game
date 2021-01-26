#include "animated_graphic.hpp"
#include <iostream>
#include "living_entity.hpp"

// circ dep
#include "entity.hpp"

Animation::Animation(SDL_Texture* spritesheet, bool no)
: spritesheet(spritesheet), no(no)
{}

Frame::Frame(int x, int y, int w, int h)
: src({x, y, w, h})
{}

Frame::~Frame()
{
	std::cout << "DEBUG: Frame deconstructor" << std::endl;
}

Animation::~Animation()
{
	std::cout << "DEBUG: Animation deconstructor" << std::endl;
}


AnimatedGraphic::AnimatedGraphic(Entity* owner)
:Graphic(owner)
{}

void AnimatedGraphic::changeState(AnimationState newState)
{
	if (curAnimationState == newState) return;
	curAnimationState = newState;
}


Sprite AnimatedGraphic::getSprite()
{
	auto animation = animations[curAnimationState].get();
	auto curFrame = animation->frames[animation->curFrame].get();
	return {animation->spritesheet, curFrame->src};
}

void AnimatedGraphic::render(SDL_Renderer* renderer, Camera* camera)
{	
	auto sprite = getSprite();
	
	if (owner->type == LIVING  && ((LivingEntity*)owner)->heldItem){
		((LivingEntity*)owner)->heldItem->graphic->render(renderer, camera);
	}

	auto pos = getPos(camera);
	if (SDL_RenderCopy( renderer, sprite.spritesheet, &sprite.src, &pos) < 0){
		std::cerr << "Failed to render sprite " + owner->name << std::endl;
	}

	frameTick();
}

void AnimatedGraphic::frameTick()
{
	if (animations.find(curAnimationState) == animations.end()){
		return;
	}
	auto animation = animations[curAnimationState].get();
	if (animation->no) return;

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
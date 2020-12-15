#include "animated_sprite.hpp"
#include <iostream>

// circ dep
#include "entity.hpp"

AnimatedSprite::AnimatedSprite(Entity* owner)
:Sprite(owner)
{}

void AnimatedSprite::render(SDL_Renderer* renderer, Camera* camera)
{
	auto animation = animations[activeAnimation].get();
	//std::cout << "DEBUG: animation " <<  animation->curFrame << std::endl;
	auto curFrame = animation->frames[animation->curFrame].get();

	auto pos = getPos(camera);
	//if (SDL_RenderCopyEx( renderer, sprite->texture, &sprite->src, &sprite->pos , NULL, NULL, sprite->flip) < 0){
	
	if (owner->behavior){
		auto lastState = owner->behavior->lastState;
		auto currentState =  owner->behavior->getState();
		if (lastState != currentState){
			//std::cout << "DEBUG: state changed to " << currentState << std::endl;
			if (animations.find(currentState) == animations.end()){
				std::cout << "DEBUG: no animation for state " << owner->name << "(" << currentState << ")" << std::endl;
				activeAnimation = ENTITY_STATE_DEFAULT;
			} else {
				activeAnimation = currentState;
			}
		}
	}

	if (SDL_RenderCopy( renderer, animation->spritesheet, &curFrame->src, &pos) < 0){
		std::cerr << "Failed to render sprite " + owner->name << std::endl;
	}

	frameTick();
}

void AnimatedSprite::frameTick()
{
	if (animations.find(activeAnimation) == animations.end()){
		return;
	}
	auto animation = animations[activeAnimation].get();
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
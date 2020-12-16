#include "animated_graphic.hpp"
#include <iostream>

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
	
	auto pos = getPos(camera);
	if (SDL_RenderCopy( renderer, sprite.spritesheet, &sprite.src, &pos) < 0){
		std::cerr << "Failed to render sprite " + owner->name << std::endl;
	}

	// if (owner->behavior){
	// 	auto animation = animations[curAnimationState].get();
	// 	auto lastState = owner->behavior->lastState;
	// 	auto curState =  owner->behavior->getState();
	// 	if (lastState != curState){
	// 		//std::cout << "DEBUG: state changed to " << curState << std::endl;
	// 		if (animations.find(curState) == animations.end()){
	// 			std::cout << "DEBUG: no animation for state " << owner->name << "(" << curState << ")" << std::endl;
	// 			curAnimationState = ENTITY_STATE_DEFAULT;
	// 		} else {
	// 			curAnimationState = curState;
	// 			animation->curFrame = 0;
	// 		}
	// 	}
	// }

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
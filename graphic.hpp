#pragma once

#include <vector>
#include "graphic.hpp"
#include "camera.hpp"
#include <map>
#include <SDL2/SDL.h>
#include <memory>

// circ dep
class Entity;

struct Sprite 
{
	SDL_Texture* spritesheet = NULL;
	SDL_Rect src = {0,0,0,0};
};

class Frame 
{
	public:
		Frame(int x, int y, int w, int h);
		~Frame();
		SDL_Rect src = { 0, 0, 0, 0 };
};

enum AnimationState
{
	DEFAULT,
	WALK,
	JUMP,
	ATTACK,
	FALL
};

class Animation
{
	public:
		Animation(SDL_Texture* spritesheet);
		~Animation();
		std::vector<std::unique_ptr<Frame>> frames;
		int curFrame = 0;
		// 0 - 20 */
		int animationSpeed = 10;
		// stores for how many frames cur animation has been shown
		int curFrameTick = 0;
		bool loop = true;
		SDL_Texture* spritesheet;
};

class Graphic
{
	public:
		/* For non-animated */
		Graphic(SDL_Texture* spritesheet, SDL_Rect src, Entity* owner);
		/* For animated */
		Graphic(Entity* owner);
		void render(SDL_Renderer* renderer, Camera* camera);
		Sprite getSprite();
		void frameTick();
		std::map<AnimationState, std::unique_ptr<Animation>> animations;
		void changeState(AnimationState newState);
		AnimationState curAnimationState = AnimationState::DEFAULT;
		SDL_Rect getPos(Camera* camera);
		const bool isAnimated;
		Entity* owner = NULL;
};
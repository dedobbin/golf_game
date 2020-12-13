#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#define ANIMATION_IDLE_INDEX 0
#define ANIMATION_WALK_INDEX 1
#define ANIMATION_JUMP_INDEX 2
#define ANIMATION_ATTACK_INDEX 3
class Frame 
{
	public:
		Frame(int x, int y, int w, int h);
		~Frame();
		SDL_Rect src = { 0, 0, 0, 0 };
		// Can later be used for rotation, store x and y of equip hand etc
};
class Animation
{
	public:
		Animation(SDL_Texture* spritesheet, bool no = false);
		~Animation();
		std::vector<std::unique_ptr<Frame>> frames;
		int curFrame = 0;
		// 0 - 20 */
		int animationSpeed = 10;
		// stores for how many frames cur animation has been shown
		int curFrameTick = 0;
		bool loop = true;
		SDL_Texture* spritesheet;
		// stops animation
		bool no = false;
};
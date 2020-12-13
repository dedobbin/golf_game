#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

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
		Animation(SDL_Texture* spritesheet);
		~Animation();
		std::vector<std::unique_ptr<Frame>> frames;
		int curFrame = 0;
		/*0 - 20 */
		int animationSpeed = 10;
		/* stores for how many frames cur animation has been shown */
		int curFrameTick = 0;
		bool loop = true;
		SDL_Texture* spritesheet;
};
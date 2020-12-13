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
};
class Animation
{
	public:
		~Animation();
		std::vector<std::unique_ptr<Frame>> frames;
		int curFrame = 0;
		bool loop = true;
};
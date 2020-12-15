#pragma once

#include <vector>
#include "sprite.hpp"

// circ dep
class Entity;

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

class AnimatedSprite : public Sprite
{
	public:
		AnimatedSprite(Entity* owner);
		void render(SDL_Renderer* renderer, Camera* camera);
		void frameTick();
		std::map<int, std::unique_ptr<Animation>> animations;
	private:
		int activeAnimation = 0;
};
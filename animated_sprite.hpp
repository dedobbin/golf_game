#pragma once

#include "sprite.hpp"

// circ dep
class Entity;

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
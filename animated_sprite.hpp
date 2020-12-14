#pragma once

#include "sprite.hpp"

// circ dep
class Entity;

class AnimatedSprite : public Sprite
{
	public:
		AnimatedSprite(Entity* owner);
		void render(SDL_Renderer* renderer);

};
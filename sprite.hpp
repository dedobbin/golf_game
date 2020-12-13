#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include <SDL2/SDL.h>
#include "camera.hpp"
#include <map>
#include <memory>
#include "animation.hpp"
// #include <SDL2/SDL_image.h>

// circ dep
class Entity;

class Sprite 
{
	public:
		Sprite(Entity* owner);
		~Sprite();
		SDL_Rect getPos(Camera* camera = NULL);
		Entity* owner;
		int activeAnimation = 0;
		std::map<int, std::unique_ptr<Animation>> animations;
};

#endif
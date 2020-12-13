#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "visuals.hpp"
#include "behavior.hpp"
#include "collision.hpp"
#include <memory>

#define ENTITY_STATE_IDLE 0
#define ENTITY_STATE_WALK 1
#define ENTITY_STATE_JUMP 4
#define ENTITY_STATE_ATTACK 8

class Entity 
{
	public:
		Entity(std::string name, int x = 0, int y = 0, int w = 100, int h = 100);
		~Entity();
		int x = 0;
		int y = 0;
		int w = 100;
		int h = 100;
		std::string name;
		std::unique_ptr<Sprite> sprite;
		std::unique_ptr<Behavior> behavior;
		std::unique_ptr<Collision> collision;
		// returns binary flags based on ENTITY_STATE defines
		int getStateFlags();
		// stores last result of getStateFlags to compare if changes happend
		int lastStateFlags = 0;
};

#endif
#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <memory>
#include "../visuals/visuals.hpp"
#include "../entity_properties/behavior.hpp"
#include "../entity_properties/collision.hpp"
#include "../utils/rect.hpp"

enum entityType {
	STATIC_SOLID, 
	LIVING,
	ITEM,
	BALL,
	SPIKES
};

class Entity 
{
	public:
		Entity(std::string name, entityType type, int x = 0, int y = 0, int w = 100, int h = 100);
		~Entity();
		rect pos;
		std::string name;
		std::unique_ptr<Graphic> graphic;
		std::unique_ptr<Behavior> behavior;
		std::unique_ptr<Collision> collision;
		entityType type;
};

#endif
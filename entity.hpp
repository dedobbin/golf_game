#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "visuals.hpp"
#include "behavior.hpp"
#include "collision.hpp"
#include "rect.hpp"
#include <memory>

enum entityType {
	STATIC_SOLID, 
	LIVING,
	ITEM,
};

class Entity 
{
	public:
		Entity(std::string name, entityType type, int x = 0, int y = 0, int w = 100, int h = 100);
		~Entity();
		/* Always returns value if not overloaded by child */
		virtual bool hasEquip();
		rect pos;
		std::string name;
		std::unique_ptr<Graphic> graphic;
		std::unique_ptr<Behavior> behavior;
		std::unique_ptr<Collision> collision;
		entityType type;
};

#endif
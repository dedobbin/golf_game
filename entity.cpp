#include "entity.hpp"
#include <iostream>

Entity::Entity(std::string name, entityType type, int x, int y, int w, int h)
: name(name), pos({x, y, w, h}), type(type)
{}

Entity::~Entity()
{
	//std::cout << "DEBUG: Entity deconstructor" << std::endl;
}

bool Entity::hasEquip()
{
	return false;
}

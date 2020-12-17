#include "entity.hpp"
#include <iostream>

Entity::Entity(std::string name, int x, int y, int w, int h)
: name(name), pos({x, y, w, h})
{}

Entity::~Entity()
{
	//std::cout << "DEBUG: Entity deconstructor" << std::endl;
}
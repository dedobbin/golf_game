#include "entity.hpp"
#include <iostream>

Entity::~Entity()
{
	std::cout << "DEBUG: Entity deconstructor" << std::endl;
}
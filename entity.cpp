#include "entity.hpp"
#include <iostream>

Entity::Entity(std::string name, int x, int y, int w, int h)
: name(name), x(x), y(y), w(w), h(h)
{}

Entity::~Entity()
{
	//std::cout << "DEBUG: Entity deconstructor" << std::endl;
}

std::string Entity::debugString()
{
	std::string res = name + ": ";
	if (sprite){
		res += "sprite|";
	}
	if (behavior){
		res += "behavior|";
	}
	if (collision){
		res += "collision|";
	}
	return res;
}
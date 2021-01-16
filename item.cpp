#include "item.hpp"
#include <iostream>

Item::Item(std::string name, int x, int y, int w, int h)
: Entity(name, ITEM, x, y, w, h)
{}

Item::~Item()
{
	//std::cout << "DEBUG: Item deconstructor" << std::endl;
}
#include "item.hpp"
#include <iostream>

Item::Item(std::string name, int w, int h, Entity* owner)
: Entity(name, ITEM, owner->pos.x, owner->pos.x, w, h)
{}

Item::Item(std::string name, int x, int y, int w, int h)
: Entity(name, ITEM, x, y, w, h)
{}

Item::~Item()
{
	std::cout << "DEBUG: Item deconstructor" << std::endl;
}
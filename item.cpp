#include "item.hpp"
#include <iostream>

Item::Item(std::string name, int x, int y, ItemType itemType, int w, int h)
: Entity(name, ITEM, x, y, w, h), itemType(itemType)
{}

Item::~Item()
{
	std::cout << "DEBUG: Item deconstructor" << std::endl;
}
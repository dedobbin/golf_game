#pragma once

#include <string>

// circ dep
class Entity;

class Item 
{
	public:
		Item(std::string name, Entity* Owner);
		std::string name;
};
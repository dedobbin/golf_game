#pragma once

#include <string>
#include "entity.hpp"

class Item : public Entity
{
	public:
		Item(std::string name, int w, int h, Entity* Owner);
		Item(std::string name, int x, int y, int w, int h);
		~Item();

};
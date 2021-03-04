#pragma once

#include <string>
#include "entity.hpp"
#include <memory>

enum ItemType
{
	CLUB
};
class Item : public Entity
{
	public:
		Item(std::string name, int x, int y, ItemType itemType, int w, int h);
		~Item();
		Entity* owner = NULL;
		ItemType itemType;
};
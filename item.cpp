#include "item.hpp"

Item::Item(std::string name, int w, int h, Entity* owner)
: Entity(name, owner->pos.x, owner->pos.x, w, h)
{}

Item::Item(std::string name, int x, int y, int w, int h)
: Entity(name, x, y, w, h)
{}
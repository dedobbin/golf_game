#include "item.hpp"

// circ dep
#include "entity.hpp"

Item::Item(std::string name, Entity* owner)
: name(name)
{}
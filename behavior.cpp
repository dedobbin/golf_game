#include "behavior.hpp"
#include "iostream"

// circ dep
#include "entity.hpp"

Behavior::Behavior(Entity* owner)
: owner(owner)
{}

void Behavior::behave()
{
	owner->x += 1;
}

Behavior::~Behavior()
{
	std::cout << "DEBUG: Behavior deconstructor" << std::endl;
}
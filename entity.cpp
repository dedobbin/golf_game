#include "entity.hpp"

Entity::Entity(Sprite* sprite)
: sprite(sprite)
{}

Entity::~Entity()
{
	delete(sprite);
}
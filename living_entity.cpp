#include "living_entity.hpp"

LivingEntity::LivingEntity(std::string name, int x, int y, int w, int h)
: Entity(name, x, y, w, h)
{}


bool LivingEntity::hasEquip()
{
	return !!heldItem;
}

#include "item_behavior.hpp"

ItemBehavior::ItemBehavior(Item* owner)
: Behavior(owner)
{}

void ItemBehavior::behave(std::vector<std::shared_ptr<Entity>> entities)
{
	if (!((Item*) owner)->owner){
		Behavior::behave(entities);
	};
}
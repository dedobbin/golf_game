#include "behavior.hpp"
#include "iostream"

#include "item.hpp"
#include "living_entity.hpp"
#include "animated_graphic.hpp"

// circ dep
#include "entity.hpp"

Behavior::Behavior(Entity* owner)
: owner(owner)
{}

void Behavior::addXSpeed(float n, bool clampZero)
{
	int prevSpeed = xSpeed;
	if (xSpeed + n > maxXSpeed){
		xSpeed = maxXSpeed;
	} else if (xSpeed + n < -maxXSpeed){
		xSpeed = -maxXSpeed;
	} else {
		xSpeed += n;
	}
	if (clampZero && round(xSpeed) == 0){
		xSpeed = 0;
	}

	if (grounded && gravity){
		if ( xSpeed != 0 && (prevSpeed > 0 && n > 0) || (prevSpeed < 0 && n < 0) ){
			((AnimatedGraphic*)owner->graphic.get())->changeState(AnimationState::WALK);
		} else {
			((AnimatedGraphic*)owner->graphic.get())->changeState(AnimationState::DEFAULT);
		}
	}

}

Behavior::~Behavior()
{
	//std::cout << "DEBUG: Behavior deconstructor" << std::endl;
}

void Behavior::addYSpeed(float n, bool clampZero)
{
	if (ySpeed + n > maxYSpeed){
		ySpeed = maxYSpeed;
	} else if (ySpeed + n < -maxYSpeed){
		ySpeed = -maxYSpeed;
	} else {
		ySpeed += n;
	}
	if (clampZero && round(ySpeed) == 0){
		ySpeed = 0;
	}
}

void Behavior::behave(std::vector<std::shared_ptr<Entity>> entities)
{
	if (owner->type == ITEM && ((Item*) owner)->owner){
		//No need to update pos, moves with owning entity
		return;
	}

	// TODO: this check all entities for collision 2 times, should optimize by sorting list, static entities on same place
	// OR only checking entities in view, but that could lead to other problems later
	// also Z will get all messed up when i resort list
	if (gravity && !grounded){
		addYSpeed(GRAVITY);
	}

	auto e = owner;

	direction hDir = NONE;
	if (xSpeed > 0){
		hDir = RIGHT;
	} else if (xSpeed < 0){
		hDir = LEFT;
	}

	owner->pos.x += xSpeed;

	for (auto& collidee : entities){
		if (e == collidee.get()) continue;
		
		auto intersect = Collision::checkCollision(e, collidee.get());
		bool collision = intersect.w > 0 && intersect.h > 0;

		if (collision){
			collidee->collision->pushout(e, hDir, intersect);
			collidee->collision->effect(e, hDir, intersect);
		}
	}

	direction vDir = NONE;
	if (ySpeed > 0){
		vDir = DOWN;
	} else if (ySpeed < 0){
		vDir = UP;
	}

	owner->pos.y += ySpeed;

	// to check if in air
	bool hasGroundUnderneath = false;
	for (auto& collidee : entities){
		if (e == collidee.get()) continue;
		
		auto intersect = Collision::checkCollision(e, collidee.get());
		bool collision = intersect.w > 0 && intersect.h > 0;

		if (collision){
			collidee->collision->pushout(e, vDir, intersect);
			collidee->collision->effect(e, vDir, intersect);
		}

		// check if solid underneath
		if (collidee->type!=ITEM 
			&& owner->collision && owner->behavior && owner->behavior->gravity
		){
			auto realPos = owner->pos;
			owner->pos.y += 1;

			auto intersect = Collision::checkCollision(e, collidee.get());
			bool groundCollision = intersect.w > 0 && intersect.h > 0;

			if (groundCollision){
				hasGroundUnderneath = true;
			}

			owner->pos = realPos;
		}
	}

	if (!hasGroundUnderneath){
		owner->behavior->grounded = false;
	}

	if (owner->type == LIVING && ((LivingEntity*)owner)->heldItem){
		auto item = ((LivingEntity*)owner)->heldItem;
		item->pos.x = owner->pos.x;
		item->pos.y = owner->pos.y;
	}


}

void Behavior::jump(){
	if (!grounded){
		return;
	}

	grounded = false;
	addYSpeed(-10.5);

}
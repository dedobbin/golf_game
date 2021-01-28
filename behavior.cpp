#include "behavior.hpp"
#include "iostream"
#include "assert.h"
#include "item.hpp"
#include "living_entity.hpp"

// circ dep
#include "entity.hpp"

Behavior::Behavior(Entity* owner, bool pickupItems)
: owner(owner), pickupItems(pickupItems)
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
			owner->graphic->changeState(AnimationState::WALK);
		} else {
			//TODO: i think this jank get called when not needed, animation looks good though..
			owner->graphic->changeState(AnimationState::DEFAULT);
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
	// TODO: this check all entities for collision 2 times, should optimize by sorting list, static entities on same place
	// OR only checking entities in view, but that could lead to other problems later
	// also Z will get all messed up when i resort list

	if (owner->collision){
		owner->collision->currentColliders = {};
	}

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
			owner->collision->currentColliders.push_back(collidee.get());
		}
	}

	direction vDir = NONE;
	if (ySpeed > 0){
		vDir = DOWN;
	} else if (ySpeed < 0){
		vDir = UP;
	}

	owner->pos.y += ySpeed;

	bool hasGroundUnder = false;
	for (auto& collidee : entities){
		if (e == collidee.get()) continue;
		
		auto intersect = Collision::checkCollision(e, collidee.get());
		bool collision = intersect.w > 0 && intersect.h > 0;

		if (collision){
			collidee->collision->pushout(e, vDir, intersect);
			collidee->collision->effect(e, vDir, intersect);
		}


		// check what is down
		auto realPos = owner->pos;
		owner->pos.y += 1;
		auto intersect2 = Collision::checkCollision(e, collidee.get());
		auto collision2 = intersect2.w > 0 && intersect2.h > 0;
		if (collision2 && !collidee->collision->isNotOrSemiSolid()){
			hasGroundUnder = true;
		}
		owner->pos = realPos;
	}

	if (!hasGroundUnder){
		grounded = false;
	}

	if (owner->type == LIVING){
		auto living = (LivingEntity*)owner;
		//held item is moved with player
		if (living->heldItem){
			auto item = living->heldItem;
			item->pos.x = owner->pos.x;
			item->pos.y = owner->pos.y;
		}

		if (living->golfMode && living->golfMode->active){
			living->golfMode->tick();
		}

		switch(living->xPush){
			case RIGHT:
				addXSpeed(grounded ? owner->behavior->walkAcc : owner->behavior->walkAcc / 2);
				break;
			case LEFT:
				addXSpeed(-(grounded ? owner->behavior->walkAcc : owner->behavior->walkAcc / 2));
				break;
			case NONE:
				if (xSpeed > 0){
					addXSpeed(-STOP_WALK_SLOW_DOWN_AMOUNT, true);
				} else if (owner->behavior->xSpeed < 0){
					addXSpeed(STOP_WALK_SLOW_DOWN_AMOUNT, true);
				}
				break;
		}
		
		// update some animations
		// If movement was stopped by wall, dont keep walk animation
		auto animatedGraphic = owner->graphic.get();
		if (animatedGraphic->curAnimationState == AnimationState::WALK && owner->behavior->xSpeed == 0){
			animatedGraphic->changeState(AnimationState::DEFAULT);
		}

		if (animatedGraphic->curAnimationState == AnimationState::JUMP && owner->behavior->grounded){
			animatedGraphic->changeState(AnimationState::DEFAULT);
		}
	}
}

void Behavior::jump(){
	//TODO: shouldn't be able to jump if something above that is not item etc
	if (!grounded){
		return;
	}

	grounded = false;
	addYSpeed(-13.5);
	owner->graphic->changeState(AnimationState::JUMP);
}
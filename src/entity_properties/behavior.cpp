#include <algorithm>
#include "iostream"
#include "assert.h"
#include "../entity_properties/behavior.hpp"
#include "../entities/item.hpp"
#include "../entities/living_entity.hpp"
#include "../world.hpp"

// circ dep
#include "../entities/entity.hpp"

Behavior::Behavior(Entity* owner, bool pickupItems)
: owner(owner), pickupItems(pickupItems)
{}

void Behavior::addXSpeed(float n, bool clampZero)
{
	int prevSpeed = xSpeed;
	float max = grounded ? maxWalkSpeed : maxXSpeed;
	if (xSpeed + n > max){
		xSpeed = max;
	} else if (xSpeed + n < -max){
		xSpeed = -max;
	} else {
		xSpeed += n;
	}
	if (clampZero && round(xSpeed) == 0){
		xSpeed = 0;
	}

	if (grounded && gravity){
		if ( xSpeed != 0 && (prevSpeed > 0 && n > 0) || (prevSpeed < 0 && n < 0) ){
			if (owner->graphic->curAnimationState == DEFAULT){
				owner->graphic->changeState(AnimationState::WALK);
			}
		} else {
			//TODO: i think this jank get called when not needed, animation looks good though..
			if (owner->graphic->curAnimationState == AnimationState::WALK){
				owner->graphic->changeState(AnimationState::DEFAULT);
			}
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

void Behavior::behave()
{
	// TODO: this check all entities for collision 2 times, should optimize by sorting list, static entities on same place
	// OR only checking entities in view, but that could lead to other problems later
	
	if (owner->pos.y < 0 || owner->pos.y > World::activeLevel->h - owner->pos.h
	|| owner->pos.x < 0 || owner->pos.x > World::activeLevel->w - owner->pos.w){
		if (owner->behavior){
			owner->behavior->destroy(false);
		}
	}


	if (destroyed){
		return;
	}
	
	if (owner->collision){
		owner->collision->currentColliders = {};
	}

	if (gravity && !grounded){
		addYSpeed(World::activeLevel->gravity);
	}

	direction hDir = NONE;
	if (xSpeed > 0){
		hDir = RIGHT;
	} else if (xSpeed < 0){
		hDir = LEFT;
	}

	owner->pos.x += xSpeed;

	for (auto& collidee : World::activeLevel->entities){// check x move
		if (owner == collidee.get()) continue;
		if (!collidee->collision) continue;
		
		auto intersect = Collision::getIntersect(owner->pos, collidee->pos);

		if (Collision::intersectCollides(intersect)){
			collidee->collision->pushout(owner, hDir, intersect);
			collidee->collision->effect(owner, hDir, intersect);
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
	for (auto& collidee : World::activeLevel->entities){// check y move + check if is grounded
		if (owner == collidee.get()) continue;
		if (!collidee->collision) continue;
		
		auto intersect = Collision::getIntersect(owner->pos, collidee->pos);

		if (Collision::intersectCollides(intersect)){
			collidee->collision->pushout(owner, vDir, intersect);
			collidee->collision->effect(owner, vDir, intersect);
		}

		// check what is down
		auto bellowPos = owner->pos;
		bellowPos.y += 1;
		auto intersect2 = Collision::getIntersect(bellowPos, collidee->pos);
		if (Collision::intersectCollides(intersect2) && !collidee->collision->isNotOrSemiSolid()){
			hasGroundUnder = true;
		}
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
				addXSpeed(grounded ? owner->behavior->walkAcc : owner->behavior->walkAcc / 4);
				break;
			case LEFT:
				addXSpeed(-(grounded ? owner->behavior->walkAcc : owner->behavior->walkAcc / 4));
				break;
			default:
				break;
		}
		/* if an entity is in the ignore map but doesn't touch owner anymore, it should be removed from ignore map */
		for (auto iter = living->ignoreEffectsList.begin(); iter != living->ignoreEffectsList.end();){
			if (std::find_if(owner->collision->currentColliders.begin(), owner->collision->currentColliders.end(), [iter](auto entity){
				return  *iter == entity;
			}) == owner->collision->currentColliders.end()){
				iter = living->ignoreEffectsList.erase(iter);
			} else {
				++iter;
			}
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

	/* friction */
	if (xSpeed > 0){
		if ( !(owner->type == LIVING && ((LivingEntity*)owner)->xPush == RIGHT) ){
			if (grounded){
				addXSpeed(-frictionGround, true);
			}
		}
	} else if (xSpeed < 0){
		if ( !(owner->type == LIVING && ((LivingEntity*)owner)->xPush == LEFT) ){
			if (grounded)
				addXSpeed(frictionGround, true);
		}
	}
}

void Behavior::jump()
{
	//TODO: shouldn't be able to jump if something above that is not item etc
	if (!grounded){
		return;
	}

	grounded = false;
	addYSpeed(-13.5);
	owner->graphic->changeState(AnimationState::JUMP);
}

void Behavior::destroy(bool animation)
{
	if (owner->type == LIVING){
		auto livingOwner = (LivingEntity*)owner;
		gravity = false;
		if (owner->collision){
			owner->collision->solid = false;
		}

		pickupItems = false;

		if (livingOwner->heldItem){
			auto item = livingOwner->heldItem;
			item->owner = NULL;
			livingOwner->heldItem = NULL;
		}
		if (owner->graphic){
			if (animation && owner->graphic->animations[AnimationState::DEAD]){
				owner->graphic->changeState(AnimationState::DEAD);
			} else {
				owner->graphic->changeState(AnimationState::NO_SPRITE);
			}
		}
	} else {
		owner->graphic->changeState(AnimationState::NO_SPRITE);
	}
	destroyed = true;
}
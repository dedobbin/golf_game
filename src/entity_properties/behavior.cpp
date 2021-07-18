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
	// This entire function is a janky mess
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
			//std::cout << "DEBUG: x collision " << owner->name << " + " << collidee->name << std::endl;
			collidee->collision->pushout(owner, hDir, intersect);
			collidee->collision->effect(owner, hDir, intersect);
			collidee->collision->currentColliders.push_back(owner);
		}
	}

	direction vDir = NONE;
	if (ySpeed > 0){
		vDir = DOWN;
	} else if (ySpeed < 0){
		vDir = UP;
	}

	owner->pos.y += ySpeed;

	std::shared_ptr<Entity> hasUnder;
	
	for (auto& collidee : World::activeLevel->entities){// check y move + check if is grounded
		
		if (owner == collidee.get()) continue;
		if (!collidee->collision) continue;
		
		auto intersect = Collision::getIntersect(owner->pos, collidee->pos);

		if (Collision::intersectCollides(intersect)){
			//std::cout << "DEBUG: y collision " << owner->name << " + " << collidee->name << std::endl;
			collidee->collision->pushout(owner, vDir, intersect);
			collidee->collision->effect(owner, vDir, intersect);
			collidee->collision->currentColliders.push_back(owner);
		}

		// check what is down TODO: wrap in functions, it's is very messy
		auto bellowPos = owner->pos;
		bellowPos.y += 1;

		if (collidee->behavior){
			if (collidee->behavior->ySpeed < 0){
				bellowPos.y -= collidee->behavior->ySpeed;
			} else if (collidee->behavior->ySpeed > 0 && owner->behavior->ySpeed < 0){
				//So you don't 'hop' up when elevator is at its upper deadpoint and goes down (without this player will be degrounded)
				bellowPos.y += collidee->behavior->ySpeed * 2;
			}
		}

		auto intersect2 = Collision::getIntersect(bellowPos, collidee->pos);
		if (Collision::intersectCollides(intersect2) && !collidee->collision->isNotOrSemiSolid()){
			hasUnder = collidee;
		}
	}

	if (!hasUnder){
		grounded = false;
	}

	/***** This is not how physics work, but yeah *****/
	if (gravity){
		yAcc = World::activeLevel->gravity;
	}

	addXSpeed(xAcc);

	//TODO: refactor this, all pretttty jank
	if (hasUnder){
		//moving up really messes with stuff, entity is ungrounded, so when that happens, make it grounded here
		if (hasUnder->type == entityType::MOVING_PLATFORM){//TODO: make more generic
			//std::cout << "DEBUG: has platform under" << std::endl;
			owner->pos.y = hasUnder->pos.y - owner->pos.h + hasUnder->behavior->ySpeed;
			if (hasUnder->behavior){
				ySpeed = hasUnder->behavior->ySpeed;
				yAcc = hasUnder->behavior->yAcc;
				setGrounded();
			}
		}else {
			ySpeed = 0;
			setGrounded();
		} 
	} else {
		addYSpeed(yAcc);
	}

	/***** specific stuff for living entities *****/
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
		if (animatedGraphic->curAnimationState == AnimationState::WALK && xSpeed == 0){
			animatedGraphic->changeState(AnimationState::DEFAULT);
		}

		if (animatedGraphic->curAnimationState == AnimationState::JUMP && grounded){
			animatedGraphic->changeState(AnimationState::DEFAULT);
		}
	}

	/***** ground friction *****/
	if (xSpeed > 0 && xAcc <= 0){
		if (grounded){
			addXSpeed(-frictionGround, true);
		}
	} else if (xSpeed < 0 && xAcc >= 0){
		if (grounded){
			addXSpeed(frictionGround, true);
		}
	}
}

void Behavior::jump()
{
	const float n = 13.5;

	//TODO: shouldn't be able to jump if something above that is not item etc.. i think that work though?
	if (!grounded){
		return;
	}

	grounded = false;
	if (ySpeed > 0){
		// Player is grounded, yet goes down, so must be on moving entity
		// taking that speed as base is wonky, just add raw
		ySpeed =- n;
	} else {
		ySpeed -= n;
	}
	justJumped = true;
	owner->graphic->changeState(AnimationState::JUMP);
}

bool Behavior::isGrounded()
{
	return grounded;
}

void Behavior::setGrounded(bool _grounded)
{
	justJumped = false;
	grounded = true;
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
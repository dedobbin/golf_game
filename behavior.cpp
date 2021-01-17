#include "behavior.hpp"
#include "iostream"
#include "assert.h"
#include "item.hpp"
#include "living_entity.hpp"
#include "animated_graphic.hpp"

// circ dep
#include "entity.hpp"

//#define CHECK_SURROUNDINGS_DEBUG
#define STOP_WALK_SLOW_DOWN_AMOUNT 0.2

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
			//TODO: i think this jank get called when not needed, animation looks good though..
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

	surroundings = {NULL, NULL, NULL, NULL};
	switch(xPush){
		case RIGHT:
			addXSpeed(owner->behavior->walkAcc);
			break;
		case LEFT:
			addXSpeed(-owner->behavior->walkAcc);
			break;
		case NONE:
			if (xSpeed > 0){
				addXSpeed(-STOP_WALK_SLOW_DOWN_AMOUNT, true);
			} else if (owner->behavior->xSpeed < 0){
				addXSpeed(STOP_WALK_SLOW_DOWN_AMOUNT, true);
			}
			break;
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

	Entity* underneath = NULL;
	Entity* above = NULL;
	Entity* left = NULL;
	Entity* right = NULL;

	for (auto& collidee : entities){
		if (e == collidee.get()) continue;
		
		auto intersect = Collision::checkCollision(e, collidee.get());
		bool collision = intersect.w > 0 && intersect.h > 0;

		if (collision){
			collidee->collision->pushout(e, vDir, intersect);
			collidee->collision->effect(e, vDir, intersect);
		}

		/*** check 4 surroundings ***/
		if (! (owner->type == LIVING && ((LivingEntity*)owner)->heldItem == collidee.get())){  
			rect intersect2;
			bool collision2;
			auto realPos = owner->pos;

			// check what is down
			owner->pos.y += 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				underneath = collidee.get();
			}
			owner->pos = realPos;
		
			// check what is up
			owner->pos.y -= 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				above = collidee.get();
			}
			owner->pos = realPos;
		
			// check what is left
			owner->pos.x -= 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				left = collidee.get();
			}
			owner->pos = realPos;
		
			// check what is right
			owner->pos.x += 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				right = collidee.get();
			}
			owner->pos = realPos;
		}
	}

	/*** end surroundings check ***/  

	//Update surroundings so overloaded function can use info to determine how enemies etc should behave
	surroundings.underneath = underneath;
	surroundings.above = above;
	surroundings.left = left;
	surroundings.right = right;


	//TODO: set grounded
	if (!surroundings.underneath 
		|| (surroundings.underneath->collision->solid && !surroundings.underneath->type == ITEM)
	){
		grounded = false;
	}

#ifdef CHECK_SURROUNDINGS_DEBUG
	// if (surroundings.underneath){
	// 	std::cout << "DEBUG: " << surroundings.underneath->name << " underneath of " << owner->name << std::endl;
	// }

	if (surroundings.above){
		std::cout << "DEBUG: " << surroundings.above->name << " above of " << owner->name << std::endl;
	}

	if (surroundings.left){
		std::cout << "DEBUG: " << surroundings.left->name << " left of " << owner->name << std::endl;
	}

	if (surroundings.right){
		std::cout << "DEBUG: " << surroundings.right->name << " right of " << owner->name << std::endl;
	}

	if (owner->type == LIVING && ((LivingEntity*)owner)->heldItem){
		auto item = ((LivingEntity*)owner)->heldItem;
		item->pos.x = owner->pos.x;
		item->pos.y = owner->pos.y;
	}
#endif

	// update some animations
	if (owner->type == LIVING){
		// If movement was stopped by wall, dont keep walk animation
		auto animatedGraphic = (AnimatedGraphic*)owner->graphic.get();
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
	addYSpeed(-10.5);
	((AnimatedGraphic*)owner->graphic.get())->changeState(AnimationState::JUMP);
}
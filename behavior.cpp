#include "behavior.hpp"
#include "iostream"
#include "assert.h"
#include "item.hpp"
#include "living_entity.hpp"
#include "animated_graphic.hpp"

// circ dep
#include "entity.hpp"

#define CHECK_SURROUNDINGS_DEBUG

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
	// TODO: this check all entities for collision 2 times, should optimize by sorting list, static entities on same place
	// OR only checking entities in view, but that could lead to other problems later
	// also Z will get all messed up when i resort list

	surroundings = {NULL, NULL, NULL, NULL, {}};
	switch(xPush){
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

	std::shared_ptr<Entity> underneath;
	std::shared_ptr<Entity> above;
	std::shared_ptr<Entity> left;
	std::shared_ptr<Entity> right;
	std::shared_ptr<Entity> inside;

	for (auto& collidee : entities){
		if (e == collidee.get()) continue;
		
		auto intersect = Collision::checkCollision(e, collidee.get());
		bool collision = intersect.w > 0 && intersect.h > 0;

		if (collision){
			collidee->collision->pushout(e, vDir, intersect);
			collidee->collision->effect(e, vDir, intersect);
			surroundings.inside.push_back(collidee);
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
				underneath = collidee;
			}
			owner->pos = realPos;
		
			// check what is up
			owner->pos.y -= 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				above = collidee;
			}
			owner->pos = realPos;
		
			// check what is left
			owner->pos.x -= 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				left = collidee;
			}
			owner->pos = realPos;
		
			// check what is right
			owner->pos.x += 1;
			intersect2 = Collision::checkCollision(e, collidee.get());
			collision2 = intersect2.w > 0 && intersect2.h > 0;
			if (collision2){
				right = collidee;
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

	if (!surroundings.underneath 
		|| (surroundings.underneath->collision && surroundings.underneath->collision->isNotOrSemiSolid())
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

	if (surroundings.inside.size() > 0){
		std::string s = "";
		for (auto& e : surroundings.inside){
			s+= e->name + " ";
		}
		std::cout << "DEBUG: " << s << "inside of " << owner->name << std::endl;

	}
#endif

	if (owner->type == LIVING){
		auto living = (LivingEntity*)owner;
		if (living->heldItem){
			auto item = living->heldItem;
			item->pos.x = owner->pos.x;
			item->pos.y = owner->pos.y;
		}
		
		// update some animations
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
	addYSpeed(-13.5);
	((AnimatedGraphic*)owner->graphic.get())->changeState(AnimationState::JUMP);
}
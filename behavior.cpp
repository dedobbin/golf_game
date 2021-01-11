#include "behavior.hpp"
#include "iostream"

// circ dep
#include "entity.hpp"

Behavior::Behavior(Entity* owner)
: owner(owner)
{}

void Behavior::addXSpeed(float n, bool clampZero)
{
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
	// TODO: this check all entities for collision twice, should optimize by sorting list, static entities on same place
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
		}
	}


	direction vDir = NONE;
	if (ySpeed > 0){
		vDir = DOWN;
	} else if (ySpeed < 0){
		vDir = UP;
	}

	owner->pos.y += ySpeed;

	for (auto& collidee : entities){
		if (e == collidee.get()) continue;
		
		auto intersect = Collision::checkCollision(e, collidee.get());
		bool collision = intersect.w > 0 && intersect.h > 0;

		if (collision){
			collidee->collision->pushout(e, vDir, intersect);
		}
	}
}

void Behavior::jump(){
	if (!grounded){
		return;
	}

	grounded = false;
	addYSpeed(-10.5);

}
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

void Behavior::behave()
{
	if (gravity && !grounded){
		addYSpeed(GRAVITY);
	}

	owner->x += xSpeed;
	owner->y += ySpeed;

	//std::cout << "DEBUG: x speed: " << xSpeed << std::endl;
}

void Behavior::jump(){
	if (!grounded){
		return;
	}

	grounded = false;
	addYSpeed(-10.5);

}

int Behavior::getState()
{
	int result = lastState;
	
	//std::cout << "DEBUG: " << (xSpeed != 0) << grounded << std::endl;

	if (gravity && ySpeed < 0){
		result = ENTITY_STATE_JUMP;
	} else if (xSpeed != 0 && grounded){
		result = ENTITY_STATE_WALK;
	} else {
		result = ENTITY_STATE_DEFAULT;
	}
	lastState = result;
	return result;
}
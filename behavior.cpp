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
		//TODO: dont use magic number, should be defined in world
		addYSpeed(0.5);
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

Behavior::~Behavior()
{
	//std::cout << "DEBUG: Behavior deconstructor" << std::endl;
}
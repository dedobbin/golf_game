#include "behavior.hpp"
#include "iostream"

// circ dep
#include "entity.hpp"

Behavior::Behavior(Entity* owner)
: owner(owner)
{}

void Behavior::addXSpeed(float n)
{
	if (xSpeed + n > maxXSpeed){
		xSpeed = maxXSpeed;
	} else if (xSpeed + n < -maxXSpeed){
		xSpeed = -maxXSpeed;
	} else {
		xSpeed += n;
	}
}

void Behavior::addYSpeed(float n)
{
	if (ySpeed + n > maxYSpeed){
		ySpeed = maxXSpeed;
	} else if (ySpeed + n < -maxYSpeed){
		ySpeed = -maxYSpeed;
	} else {
		ySpeed += n;
	}
}

void Behavior::behave()
{
	owner->x += xSpeed;
	owner->y += ySpeed;

	std::cout << "DEBUG: x speed: " << xSpeed << std::endl;
}

Behavior::~Behavior()
{
	std::cout << "DEBUG: Behavior deconstructor" << std::endl;
}
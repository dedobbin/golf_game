#include "behavior.hpp"
#include "iostream"

// circ dep
#include "entity.hpp"

Behavior::Behavior(Entity* owner)
: owner(owner)
{}

void Behavior::addXSpeed(float n)
{
	xSpeed = xSpeed + n > maxXSpeed ? maxXSpeed : xSpeed + n;
}

void Behavior::addYSpeed(float n)
{
	ySpeed = ySpeed + n > maxYSpeed ? maxYSpeed : ySpeed + n;
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
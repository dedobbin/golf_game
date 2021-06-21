#include "moving_platform_behavior.hpp"
#include "iostream"

MovingPlatformBehavior::MovingPlatformBehavior(Entity* owner, rect endPos, int speed)
: Behavior(owner), endPos(endPos), speed(speed)
{}

void MovingPlatformBehavior::behave()
{
	Behavior::behave();
    //TODO
}
#include "moving_platform_behavior.hpp"
#include "iostream"

MovingPlatformBehavior::MovingPlatformBehavior(Entity* owner, rect endPos, int speed)
: Behavior(owner), endPos(endPos), speed(speed)
{
    std::cout << "new" << std::endl;
}

void MovingPlatformBehavior::behave()
{
	Behavior::behave();
    //TODO
    
    // std::cout << owner->pos.y << " to " << endPos.y << std::endl;

    // if (owner->pos.y < endPos.y){
    //     if (curDir == NONE){
    //         std::cout << "DEBUG moving_platform_behavior: should move up" << std::endl;
    //         addYSpeed(-speed);
    //     }
    // } else {
    //     curDir = NONE;
    //     ySpeed = 0;
    // }
}
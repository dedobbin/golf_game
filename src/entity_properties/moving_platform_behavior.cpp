#include "moving_platform_behavior.hpp"
#include "iostream"

MovingPlatformBehavior::MovingPlatformBehavior(Entity* owner, rect endPos, int speed)
: Behavior(owner), endPos(endPos), speed(speed), curDestination(endPos)
{
    std::cout << "new" << std::endl;
}

void MovingPlatformBehavior::behave()
{
	Behavior::behave();
    //TODO
    
    //std::cout << owner->pos.y << " to " << endPos.y << std::endl;

    direction shouldMoveYDir = owner->pos.y == curDestination.y ? NONE : owner->pos.y > curDestination.y ? UP : DOWN; 
    
    if (curYDir == NONE){
        if (owner->pos.y > curDestination.y){
            addYSpeed(-speed);
            curYDir = UP;
        } 
    } else if (curYDir == UP){
        if (owner->pos.y <= curDestination.y){
            ySpeed = 0;
            curYDir = NONE;

            //swap curDestinations
            if (curDestination.x == owner->pos.x && curDestination.y == owner->pos.y){
                curDestination = endPos;
            } else {
                curDestination = owner->pos;
            }
        }
    }
}
#include "moving_platform_behavior.hpp"
#include "iostream"

MovingPlatformBehavior::MovingPlatformBehavior(Entity* owner, rect endPos, int speed)
: Behavior(owner), endPos(endPos), speed(speed), curDestination(endPos), startPos(owner->pos)
{
    std::cout << "new" << std::endl;
}

void MovingPlatformBehavior::behave()
{
	Behavior::behave();
    //direction shouldMoveYDir = owner->pos.y == curDestination.y ? NONE : owner->pos.y > curDestination.y ? UP : DOWN; 
    
    if (curYDir == NONE){
        if (owner->pos.y > curDestination.y){
            addYSpeed(-speed);
            curYDir = UP;
        } else if (owner->pos.y <= curDestination.y){
            addYSpeed(speed);
            curYDir = DOWN;
        }
    } else if (curYDir == UP){
        if (owner->pos.y <= curDestination.y){
            ySpeed = 0;
            curYDir = NONE;

            //swap curDestinations
            if (curDestination.x == endPos.x && curDestination.y == endPos.y){
                curDestination = startPos;
            } else {
                curDestination = endPos;
            }
        }
    } else if (curYDir == DOWN){
        if (owner->pos.y >= curDestination.y){
            std::cout << "DEBUG reached" << owner->pos.y << std::endl;
            ySpeed = 0;
            curYDir = NONE;
        }
    }
}
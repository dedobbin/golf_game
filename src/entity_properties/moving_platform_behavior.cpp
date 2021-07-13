#include "moving_platform_behavior.hpp"
#include "iostream"

MovingPlatformBehavior::MovingPlatformBehavior(Entity* owner, rect endPos, float acc)
: Behavior(owner), endPos(endPos), potentionalAcc(acc), curDestination(endPos), startPos(owner->pos)
{
    // TODO: move left and right etc
    if (endPos.x != startPos.x){
        std::cout << "MovingPlatformBehavior currently supports going up and down, correcting endPos.x to " << startPos.x << std::endl;
        endPos.x = startPos.x;
    }
}

void MovingPlatformBehavior::behave()
{
	Behavior::behave();
    //direction shouldMoveYDir = owner->pos.y == curDestination.y ? NONE : owner->pos.y > curDestination.y ? UP : DOWN; 
    
    if (ySpeed == 0){
        if (curDestination.y < owner->pos.y){
            yAcc = -potentionalAcc;
        } else {
            yAcc = potentionalAcc;
        }
    } else if ((ySpeed < 0 && curDestination.y >= owner->pos.y)
    || ySpeed > 0 && curDestination.y <= owner->pos.y){
        ySpeed = 0;
        yAcc = 0;
        //swap curDestinations
        curDestination = (curDestination.x == endPos.x && curDestination.y == endPos.y) ? startPos : endPos;
    } 

    // if (curYDir == NONE){
    //     if (owner->pos.y > curDestination.y){
    //         yAcc = -speed;
    //         curYDir = UP;
    //         std::cout << "DEBUG platform: should go up:" << yAcc << std::endl;
    //     } else if (owner->pos.y <= curDestination.y){
    //         std::cout << "DEBUG platform: should go down" << std::endl;
    //         yAcc = speed;
    //         curYDir = DOWN;
    //     }
    // } else if (curYDir == UP){
    //     if (owner->pos.y <= curDestination.y){
    //         std::cout << "DEBUG platform: reached" << std::endl;
    //         yAcc = 0;
    //         ySpeed = 0;
    //         curYDir = NONE;

    //         //swap curDestinations
    //         if (curDestination.x == endPos.x && curDestination.y == endPos.y){
    //             curDestination = startPos;
    //         } else {
    //             curDestination = endPos;
    //         }
    //     }
    // } else if (curYDir == DOWN){
    //     if (owner->pos.y >= curDestination.y){
    //         std::cout << "DEBUG platform: reached" << std::endl;
    //         yAcc = 0;
    //         ySpeed = 0;
    //         curYDir = NONE;
    //     }

    //     //swap curDestinations
    //     if (curDestination.x == endPos.x && curDestination.y == endPos.y){
    //         curDestination = startPos;
    //     } else {
    //         curDestination = endPos;
    //     }

    // }
    //std::cout << "DEBUG platform: " << yAcc << " , " << ySpeed << std::endl;
}
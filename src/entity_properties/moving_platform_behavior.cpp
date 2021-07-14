#include "moving_platform_behavior.hpp"
#include "iostream"

MovingPlatformBehavior::MovingPlatformBehavior(Entity* owner, rect endPos)
: Behavior(owner), endPos(endPos), curDestination(endPos), startPos(owner->pos)
{
    gravity = false;//Default gravity to false, otherwise platforms wont move
    maxYSpeed = 5;
    potentionalAcc = 10;
    
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
        yAcc = curDestination.y < owner->pos.y ? -potentionalAcc : potentionalAcc;
    } 

    if (curDestination.y >= owner->pos.y){
        if (ySpeed < 0){
            curDestination = (curDestination.x == endPos.x && curDestination.y == endPos.y) ? startPos : endPos;
            yAcc = potentionalAcc;
        }
    } else if (curDestination.y <= owner->pos.y){
        if (ySpeed > 0){
            curDestination = (curDestination.x == endPos.x && curDestination.y == endPos.y) ? startPos : endPos;
            yAcc = -potentionalAcc;
        }
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
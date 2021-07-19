#include "camera.hpp"
#include <iostream>

/* circ dep, could get rid of this by doing entity->followWithCam(camera) */
#include "../entities/entity.hpp"

Camera::Camera(int x, int y, int w, int h, CameraType type)
:camRect({x, y, w, h}), type(type)
{}

bool Camera::inView(int x, int y, int w, int h)
{
    //Calculate the sides of rect A
    int leftA = camRect.x;
    int rightA = camRect.x + camRect.w;
    int topA = camRect.y;
    int bottomA = camRect.y + camRect.h;
        
    //Calculate the sides of rect B
    int leftB = x;
    int rightB = x + w;
    int topB = y;
    int bottomB = y + h;

  	//If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }
    
    if( topA >= bottomB )
    {
        return false;
    }
    
    if( rightA <= leftB )
    {
        return false;
    }
    
    if( leftA >= rightB )
    {
        return false;
    }
    
    //If none of the sides from A are outside B
    return true;
}

bool Camera::partiallyInView(int x, int y, int w, int h)
{
    int leftA = camRect.x;
    int rightA = camRect.x + camRect.w;
    int topA = camRect.y;
    int bottomA = camRect.y + camRect.h;
        
    int leftB = x;
    int rightB = x + w;
    int topB = y;
    int bottomB = y + h;

    if ((rightB > rightA && leftB < rightA)
    || (leftB < leftA && rightB > leftA)
    || (topB < topA && bottomB > topA) 
    || (bottomB > bottomA && topB < bottomA)
    ){
        return true;
    }

    return false;
}

void Camera::followWithCam(std::shared_ptr<Entity> entity)
{
    if (type == CameraType::NO_FOLLOW){        
        return;
    } else if (type == CameraType::CENTER){
        snapToSanePos(entity);
    } else if (type == CameraType::FOLLOW){ 
        //TODO: make wrapper functions to get these 'states'
        
        int fallFromJumpTreshold = 50;
        if (!entity->behavior->isGrounded() && entity->behavior->ySpeed > 0){
            // falling
            int downSpace = 100;
            if ((camRect.y + camRect.h) - (entity->pos.y + entity->pos.h) < downSpace
            || (entity->behavior->getJustJumped() 
            && entity->pos.y - entity->behavior->getLastJumpedFrom().y > fallFromJumpTreshold)){
                camRect.y += entity->behavior->ySpeed;
            }

        } else if(entity->behavior->isGrounded() && !entity->behavior->ySpeed == 0){
            //elevator
            int downSpace = 100;
            int upSpace = 100;
            if (entity->pos.y + entity->pos.h > camRect.y + camRect.h - downSpace){
                //std::cout << "DEBUG: camera goes down " << rand() %10 << std::endl;
                camRect.y += entity->behavior->ySpeed;   
            } else if (entity->pos.y < camRect.y + upSpace){
                //std::cout << "DEBUG: camera goes up " << rand()%10;
                camRect.y += entity->behavior->ySpeed;
            }
        } else if (!entity->behavior->isGrounded() && entity->behavior->ySpeed < 0){
            //jumping
            int upSpace = 80;
            if (entity->pos.y < camRect.y + upSpace){
                camRect.y += entity->behavior->ySpeed;
            }

        }

        // else if (entity->behavior->isGrounded() && entity->behavior->ySpeed == 0){
        //     // on stable ground, alot of junk, luckily we dont use it anymore
        //     int a = entity->pos.y + entity->pos.h;
        //     int b = camRect.y + camRect.h;
        //     int camSpeed = 4;
        //     int downSpace = 200;
        //     int treshold = 100; //if pos was moved less than this, ignore otherwise snaps with alot of jumps..
        //     int diff = ((camRect.y + camRect.h) - (entity->pos.y + entity->pos.h)) - downSpace;
        //     /* TODO: fix this terrible code */
        //     // TODO: don't do this, if change is small - then do another check, to check of many small changes didn't mess with camera
        //     if (diff < -treshold){
        //         //Checks if will overshoot, will cause spazzing
        //         if (!((camRect.y + camRect.h + camSpeed) - (entity->pos.y + entity->pos.h) > downSpace)){
        //             camRect.y += camSpeed;
        //         }
        //         camRect.y += camSpeed;
        //     } else if (diff > treshold){
        //         //Checks if will overshoot, will cause spazzing
        //         if (!((camRect.y + camRect.h - camSpeed) - (entity->pos.y + entity->pos.h) < downSpace)){
        //             camRect.y -= camSpeed;
        //         }
        //     }
        // } 

        int rightSpace = 400;
        int leftSpace = 200;
        if (entity->pos.x + entity->pos.w > camRect.x + camRect.w - rightSpace){
            if (entity->behavior->xSpeed > 0){
                camRect.x += entity->behavior->xSpeed;
            }
        } else if (entity->pos.x < camRect.x + leftSpace){
            if (entity->behavior->xSpeed < 0){
                camRect.x += entity->behavior->xSpeed;
            }
        }
    }

}

void Camera::snapToSanePos(std::shared_ptr <Entity> entity)
{
    camRect.x = entity->pos.x - camRect.w/3; 
	camRect.y = entity->pos.y - camRect.h + camRect.h / 2;
}
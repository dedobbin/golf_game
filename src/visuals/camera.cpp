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
        //todo
  
        // TODO: don't move cam outside of world edges 
        // TODO: check if entity doesn't fit in cam/work with cam logic because too big, always hits edges?
        auto pos = entity->pos;

        if (!entity->behavior || partiallyInView(pos.x, pos.y, pos.w, pos.h) || !inView(pos.x, pos.y, pos.w, pos.h)){
        	snapToSanePos(entity);
        	return;
        }

        int uSpace = camRect.h / 3;
        int dSpace = camRect.h / 3;
        if (camRect.y + camRect.h < pos.y + pos.h + uSpace){
            camRect.y += entity->behavior->ySpeed;
        } else if (camRect.y > pos.y - dSpace){
        	camRect.y += entity->behavior->ySpeed;
        }

        int rSpace = camRect.w / 2;
        int lSpace = camRect.w / 3;
        if (camRect.x + camRect.w < pos.x + pos.w + rSpace && entity->behavior->xSpeed > 0){
        	camRect.x += entity->behavior->xSpeed;
        }else if (camRect.x > pos.x -lSpace && entity->behavior->xSpeed < 0){
        	camRect.x += entity->behavior->xSpeed;
        }
    }

    //TODO: use commented out code instead of snapping everytime
    // snapToSanePos(entity);

	//TODO: don't move cam outside of world edges 
	//TODO: check if entity doesn't fit in cam/work with cam logic because too big, always hits edges?
    // auto pos = entity->pos;

	// if (snapIfOutsideOfView &&
	// (!entity->behavior || partiallyInView(pos.x, pos.y, pos.w, pos.h) || !inView(pos.x, pos.y, pos.w, pos.h))
	// ){
	// 	snapToSanePos(entity);
	// 	return;
	// }

    // int uSpace = camRect.h / 6;
	// int dSpace = camRect.h / 7;
	// if (camRect.y + camRect.h < pos.y + pos.h + uSpace){
    //     camRect.y += entity->behavior->ySpeed;
	// } else if (camRect.y > pos.y - dSpace){
	// 	camRect.y += entity->behavior->ySpeed;
	// }

	// int rSpace = camRect.w / 2;
	// int lSpace = camRect.w / 3;
	// if (camRect.x + camRect.w < pos.x + pos.w + rSpace && entity->behavior->xSpeed > 0){
	// 	camRect.x += entity->behavior->xSpeed;
	// }else if (camRect.x > pos.x -lSpace && entity->behavior->xSpeed < 0){
	// 	camRect.x += entity->behavior->xSpeed;
	// }
}

void Camera::snapToSanePos(std::shared_ptr <Entity> entity)
{
    camRect.x = entity->pos.x - camRect.w/3; 
	camRect.y = entity->pos.y - camRect.h + camRect.h / 2;
}
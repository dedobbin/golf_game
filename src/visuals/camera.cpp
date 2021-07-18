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
        // if (entity->behavior->grounded && entity->behavior->ySpeed == 0){
        //     std::cout << "DEBUG: on stable ground" << std::endl;
        // } else if (entity->behavior->grounded && !entity->behavior->ySpeed == 0){
        //     std::cout << "DEBUG: on elevator" << std::endl;
        // } else if (!entity->behavior->grounded && entity->behavior->ySpeed > 0){
        //     std::cout << "DEBUG: falling" << std::endl;
        // } else if (!entity->behavior->grounded && entity->behavior->ySpeed < 0){
        //     std::cout << "DEBUG: jumping" << std::endl;
        // }


        if (entity->behavior->grounded && entity->behavior->ySpeed == 0){
            // on stable ground
            // int a = entity->pos.y + entity->pos.h;
            // int b = camRect.y + camRect.h;
            int camSpeed = 10;
            int downSpace = 200;
            /* TODO: fix this terrible code*/
            if ((camRect.y + camRect.h) - (entity->pos.y + entity->pos.h) < downSpace){
                //Checks if will overshoot, will cause spazzing, TODO: fix this terrible code
                if (!((camRect.y + camRect.h + camSpeed) - (entity->pos.y + entity->pos.h) > downSpace)){
                    camRect.y += camSpeed;
                }
                camRect.y += camSpeed;
            } else if ((camRect.y + camRect.h) - (entity->pos.y + entity->pos.h) > downSpace){
                //Checks if will overshoot, will cause spazzing
                if (!((camRect.y + camRect.h - camSpeed) - (entity->pos.y + entity->pos.h) < downSpace)){
                    camRect.y -= camSpeed;
                }
            }
        } else if (entity->behavior->grounded && !entity->behavior->ySpeed == 0 || !entity->behavior->grounded && entity->behavior->ySpeed > 0){
            // falling or on elevator
            int downSpace = 100;
            int upSpace = 100;
            if (entity->pos.y + entity->pos.h > camRect.y + camRect.h - downSpace){
                //std::cout << "DEBUG: camera goes down " << rand() %10 << std::endl;
                camRect.y += entity->behavior->ySpeed;   
            } else if (entity->pos.y < camRect.y + upSpace){
                //std::cout << "DEBUG: camera goes up " << rand()%10;
                camRect.y += entity->behavior->ySpeed;
                std::cout << "DEBUG: jank when not on elevator" << std::endl;
            }
        } else if (!entity->behavior->grounded && entity->behavior->ySpeed < 0){
            //jumping
            int upSpace = 50;
            if (entity->pos.y < camRect.y - upSpace){
                camRect.y += entity->behavior->ySpeed;
            }

        }


        //if grounded and y is not moving, on ground
            //stabilize based on under player == 100, 2 conditionals
        //if grounded and moving OR falling 
            //go with flow


        // if (!entity->behavior->ySpeed > 0){
        //     if (b - a < downSpace){
        //         camRect.y += camSpeed;
        //     } else {
        //         std::cout << "player " << a;
        //         std::cout << ",cam " << b;
        //         std::cout << "(" << b-a << ")" << std::endl; 
        //     }
        // } else if (entity->behavior->grounded){
        //     int downSpace = 50;
        //     int upSpace = 50;
        //     if (entity->pos.y + entity->pos.h > camRect.y + camRect.h - downSpace){
        //         //std::cout << "DEBUG: camera goes down " << rand() %10 << std::endl;
        //         camRect.y += entity->behavior->ySpeed;   
        //     } else if (entity->pos.y < camRect.y + upSpace){
        //         //std::cout << "DEBUG: camera goes up " << rand()%10;
        //         camRect.y += entity->behavior->ySpeed;
        //     }
        // }
        
        
        // int downSpace = 50;
        // int upSpace = 50;
        // if (entity->pos.y + entity->pos.h > camRect.y + camRect.h - downSpace){
        //     //std::cout << "DEBUG: camera goes down " << rand() %10 << std::endl;
        //     camRect.y += entity->behavior->ySpeed;   
        // } else if (entity->pos.y < camRect.y + upSpace){
        //     //std::cout << "DEBUG: camera goes up " << rand()%10;
        //     camRect.y += entity->behavior->ySpeed;
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



  
        // TODO: don't move cam outside of world edges 
        // TODO: check if entity doesn't fit in cam/work with cam logic because too big, always hits edges?
        // auto pos = entity->pos;

        // if (!entity->behavior || partiallyInView(pos.x, pos.y, pos.w, pos.h) || !inView(pos.x, pos.y, pos.w, pos.h)){
        // 	snapToSanePos(entity);
        // 	return;
        // }

        // int uSpace = camRect.h / 3;
        // int dSpace = camRect.h / 3;
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
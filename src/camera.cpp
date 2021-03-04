#include "camera.hpp"

Camera::Camera(int x, int y, int w, int h)
:camRect({x, y, w, h})
{}

bool Camera::inView(int x, int y, int w, int h)
{
   	//The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = camRect.x;
    rightA = camRect.x + camRect.w;
    topA = camRect.y;
    bottomA = camRect.y + camRect.h;
        
    //Calculate the sides of rect B
    leftB = x;
    rightB = x + w;
    topB = y;
    bottomB = y + h;

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
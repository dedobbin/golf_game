#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

//circ dep
#include "entity.hpp"

Collision::Collision(Entity* owner, bool solid)
:owner(owner), solid(solid)
{}

Collision::~Collision()
{
	//std::cout << "DEBUG: Collision deconstructor" << std::endl;
}

bool Collision::checkCollision(rect a, rect b)
{
    // thanks lazy foo, https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;


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

bool Collision::pushout(Entity* collider, direction colliderDir, rect intersect)
{
    bool bothSolid = collider->collision->solid && owner->collision->solid;

    if (bothSolid){
        rect ownerRect = owner->collision->getRect();
        rect colliderRect = collider->collision->getRect();
        if (collider->behavior){
            switch(colliderDir){
                case DOWN:
                    collider->pos.y -= intersect.h;
                    collider->behavior->ySpeed = 0;
                    if (collider->behavior->gravity){
                        collider->behavior->grounded = true;
                    }
                    break;
                case UP:
                    collider->pos.y += intersect.h;
                    collider->behavior->ySpeed = 0;
                    break;
                case LEFT:
                    collider->pos.x += intersect.w;
                    collider->behavior->xSpeed = 0;
                    break;
                case RIGHT:
                    collider->pos.x -= intersect.w;
                    collider->behavior->xSpeed = 0;
                    break;
            }
        }
    }
}

rect Collision::getRect()
{
    // TODO: make dynamic
    int wMargin = 3;
    int hMargin = 0;

    return {
        owner->pos.x + wMargin,
        owner->pos.y + hMargin,
        owner->pos.w - wMargin * 2,
        owner->pos.h - hMargin * 2
    };
}
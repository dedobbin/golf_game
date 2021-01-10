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

rect Collision::checkCollision(Entity* entityA, Entity* entityB)
{
	rect intersect = {0,0,0,0};

	if (!entityA->collision || !entityB->collision){
		return intersect;
	}

    rect a = entityA->collision->getRect();
    rect b = entityB->collision->getRect();
	
	//Based on SDL_IntersectRect

    int Amin, Amax, Bmin, Bmax;	
	/* Horizontal intersection */
    Amin = a.x;
    Amax = Amin + a.w;
    Bmin = b.x;
    Bmax = Bmin + b.w;
    if (Bmin > Amin)
        Amin = Bmin;
    intersect.x = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    intersect.w = Amax - Amin;

    /* Vertical intersection */
    Amin = a.y;
    Amax = Amin + a.h;
    Bmin = b.y;
    Bmax = Bmin + b.h;
    if (Bmin > Amin)
        Amin = Bmin;
    intersect.y = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    intersect.h = Amax - Amin;

	return intersect; 
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

void Collision::effect(Entity* collider, rect intersect)
{
    bool bothSolid = collider->collision->solid && owner->collision->solid;

    //pushing out of solid objects
    if (bothSolid && collider->behavior){
        // int yMove = collider->pos.y - collider->prevPos.y;
        // int xMove = collider->pos.x - collider->prevPos.x;

        if (collider->collision->getRect().w / intersect.w < collider->collision->getRect().h / intersect.h){
            if (collider->pos.y > collider->prevPos.y){
                collider->pos.y -= intersect.h;
                collider->behavior->ySpeed = 0;


                if (collider->behavior->gravity){
                    collider->behavior->grounded = true;
                }

            } else if (collider->pos.y < collider->prevPos.y){
                collider->pos.y += intersect.h;
                collider->behavior->ySpeed = 0;
            }
        }else {
            if (collider->pos.x > collider->prevPos.x){
                collider->pos.x -= intersect.w;
                collider->behavior->xSpeed = 0;
            } else if (collider->pos.x < collider->prevPos.x){
                collider->pos.x += intersect.w;
                collider->behavior->xSpeed = 0;
            }
        }
    }//TODO: handle non behavior push out? when spawned at some place etc
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
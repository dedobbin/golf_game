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

void Collision::effect(Entity* collider, rect intersect)
{
    bool bothSolid = collider->collision->solid && owner->collision->solid;
    if (collider->behavior && bothSolid){
        if (intersect.h > intersect.w){
            if (collider->behavior->xSpeed < 0 && collider->x > owner->x){
                collider->x += intersect.w;
                collider->behavior->xSpeed = 0;
            } else if (collider->behavior->xSpeed > 0 && collider->x < owner->x){
                collider->x -= intersect.w;
                collider->behavior->xSpeed = 0;
            }
        } else {
            if (collider->behavior->ySpeed < 0 && collider->y > owner->y){
                collider->y += intersect.h;
                collider->behavior->ySpeed = 0;
            } else if (collider->behavior->ySpeed > 0 && collider->y < owner->y){
                collider->y -= intersect.h;
                collider->behavior->ySpeed = 0;
            }
        }
    }
}

rect Collision::getRect()
{
    return {
        owner->x + 10,
        owner->y + 10,
        owner->w - 20,
        owner->h - 20
    };
}
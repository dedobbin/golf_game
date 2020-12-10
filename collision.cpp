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

bool Collision::checkCollision(Entity* a, Entity* b)
{
	if (!a->collision || !b->collision){
		return false;
	}
	
	//Based on SDL_IntersectRect
	rect intersect;
    int Amin, Amax, Bmin, Bmax;	
	/* Horizontal intersection */
    Amin = a->x;
    Amax = Amin + a->w;
    Bmin = b->x;
    Bmax = Bmin + b->w;
    if (Bmin > Amin)
        Amin = Bmin;
    intersect.x = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    intersect.w = Amax - Amin;

    /* Vertical intersection */
    Amin = a->y;
    Amax = Amin + a->h;
    Bmin = b->y;
    Bmax = Bmin + b->h;
    if (Bmin > Amin)
        Amin = Bmin;
    intersect.y = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    intersect.h = Amax - Amin;

	bool collision = intersect.w > 0 && intersect.h > 0;

	if (collision){
		b->collision->effect(a, intersect);
	} 

	return collision; 
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
                collider->behavior->grounded = true;
                std::cout << "DEBUG: landed" << std::endl; 
            }
        }
    }
}
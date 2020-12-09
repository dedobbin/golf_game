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
	int intersect[4] = {0, 0, 0, 0}; //x,y,w,h
    int Amin, Amax, Bmin, Bmax;	
	/* Horizontal intersection */
    Amin = a->x;
    Amax = Amin + a->w;
    Bmin = b->x;
    Bmax = Bmin + b->w;
    if (Bmin > Amin)
        Amin = Bmin;
    intersect[0] = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    intersect[2] = Amax - Amin;

    /* Vertical intersection */
    Amin = a->y;
    Amax = Amin + a->h;
    Bmin = b->y;
    Bmax = Bmin + b->h;
    if (Bmin > Amin)
        Amin = Bmin;
    intersect[1] = Amin;
    if (Bmax < Amax)
        Amax = Bmax;
    intersect[3] = Amax - Amin;

	bool collision = intersect[2] > 0 && intersect[3] > 0;

	if (collision){
        //std::cout << "DEBUG: " << a->name << " collides with " << b->name << std::endl;
		b->collision->effect(a, intersect[0], intersect[1], intersect[2], intersect[3]);
	}
	return collision; 
}

void Collision::effect(Entity* collider, int intersectX, int intersectY, int intersectW, int intersectH)
{
    bool bothSolid = collider->collision->solid && owner->collision->solid;
    if (collider->behavior && bothSolid){
        if (intersectH > intersectW){
            if (collider->behavior->xSpeed < 0 && collider->x > owner->x){
                collider->x += intersectW;
                collider->behavior->xSpeed = 0;
            } else if (collider->behavior->xSpeed > 0 && collider->x < owner->x){
                collider->x -= intersectW;
                collider->behavior->xSpeed = 0;
            }
        } else {
            if (collider->behavior->ySpeed < 0 && collider->y > owner->y){
                collider->y += intersectH;
                collider->behavior->ySpeed = 0;
            } else if (collider->behavior->ySpeed > 0 && collider->y < owner->y){
                collider->y -= intersectH;
                collider->behavior->ySpeed = 0;
            }
        }
    }
}
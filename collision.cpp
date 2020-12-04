#include <iostream>
#include <SDL2/SDL.h>

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
        std::cout << "DEBUG: " << a->name << " collides with " << b->name << std::endl;
		b->collision->effect(a, intersect[0], intersect[1], intersect[2], intersect[3]);
	}
	return collision; 
}

void Collision::effect(Entity* collider, int intersectX, int intersectY, int intersectW, int intersectH)
{
	//TODO: effects like damage, i guess make child classes so don't dump everything here
    
    //'push' out
	if (collider->collision->solid && solid){
        //std::cout << "TODO: push out" << std::endl;
        // if (intersectW > 0){
        //     if (intersectX == owner->x){
        //         if (collider->behavior && collider->behavior->xSpeed != 0){
        //             //comes from left side
        //             //std::cout << "collision left " << std::endl;
        //             collider->x = owner->x - collider->w;
        //         }
        //     } else {
        //         if (collider->behavior && collider->behavior->xSpeed != 0){
        //             //comes from right side
        //             //std::cout << "collision right " << std::endl;
        //             collider->x = owner->x + owner->w;
        //         }
        //     }
        // }
	}
}
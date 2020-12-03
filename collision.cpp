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
		b->collision->effect(a);
	}

	//'push' out
	if (collision && a->collision->solid && b->collision->solid){
		std::cout << "TODO: push out" << std::endl;
		
	}
	return true; 
}

void Collision::effect(Entity* collider)
{
	//TODO: effects like damage, i guess make child classes so don't dump everything here
}
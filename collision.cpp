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

    int leftA = a->x;
    int rightA = a->x + a->w;
    int topA = a->y;
    int bottomA = a->y + a->h;
        
    int leftB = b->x;
    int rightB = b->x + b->w;
    int topB = b->y;
    int bottomB = b->y + b->h;

    if( bottomA < topB )
    {
        return false;
    }
    
    if( topA > bottomB )
    {
        return false;
    }
    
    if( rightA < leftB )
    {
        return false;
    }
    
    if( leftA > rightB )
    {
        return false;
    }
    
	// 'push' out
	if (a->collision->solid && b->collision->solid){
		if (a->behavior){
			if (a->behavior->ySpeed > 0){
				a->y = b->y - a->h;
				a->behavior->setGrounded();
			}
		}
	}

	b->collision->effect(a);

	return true; 
}

void Collision::effect(Entity* collider)
{
	//TODO: effects like damage, i guess make child classes so don't dump everything here
}
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

#include "item.hpp"
#include "living_entity.hpp"

//circ dep
#include "entity.hpp"

Collision::Collision(Entity* owner, bool solid)
:owner(owner), solid(solid)
{}

Collision::~Collision()
{
	//std::cout << "DEBUG: Collision deconstructor" << std::endl;
}

rect Collision::getIntersect(rect a, rect b)
{
	rect intersect = {0,0,0,0};
	
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

bool Collision::intersectCollides(rect intersect)
{
    return intersect.w > 0 && intersect.h > 0;
}

void Collision::pushout(Entity* collider, direction colliderDir, rect intersect)
{
    if (!collider->collision){
        return;
    }

    if ((isNotOrSemiSolid() && ((collider->collision->isNotOrSemiSolid() || collider->type == LIVING)) 
        || (isNotOrSemiSolid() || owner->type == LIVING) && collider->collision->isNotOrSemiSolid())){
        return;
    }

    // dont mess with entities in golfmode
    if (collider->type == LIVING && ((LivingEntity*)collider)->golfMode && ((LivingEntity*)collider)->golfMode->active){
        return;
    }

    rect ownerRect = owner->pos;
    rect colliderRect = collider->pos;

    bool bothSolid = collider->collision->solid && owner->collision->solid;

    if (collider->behavior){
        switch(colliderDir){
            case DOWN: case NONE:
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

bool Collision::effect(Entity* collider, direction colliderDir, rect intersect)
{
    if (collider->type == LIVING){
        switch(owner->type){
            case ITEM:
                //if not owned by an entity, its on the field
                auto item = (Item*)owner;
                if (!item->owner && collider->behavior->pickupItems){
                    auto living = (LivingEntity*) collider;
                    living->give(item);
                }
                return false;
        }
        return false;
    }
}

bool Collision::isNotOrSemiSolid()
{
    if (!solid){
        return true;
    }
    if (owner->type == ITEM){
        return true;
    }
    if (owner->type == BALL){
	//if (owner->type == BALL && (owner->behavior->xSpeed == 0 && owner->behavior->ySpeed == 0)){
        return true;
    }
}
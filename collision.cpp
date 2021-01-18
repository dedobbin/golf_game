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

void Collision::pushout(Entity* collider, direction colliderDir, rect intersect)
{
    if ((owner->type == ITEM && collider->type == LIVING) 
        || owner->type == LIVING && collider->type == ITEM){
        return;
    }

    rect ownerRect = owner->collision->getRect();
    rect colliderRect = collider->collision->getRect();

    bool bothSolid = collider->collision->solid && owner->collision->solid;

    if (bothSolid){
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

bool Collision::isNotOrSemiSolid()
{
    if (!solid){
        return true;
    }
    if (owner->type == ITEM && !((Item*)owner)->owner){
        return true;
    }
	if (owner->type == BALL && (owner->behavior->xSpeed == 0 && owner->behavior->ySpeed == 0)){
        return true;
    }
}
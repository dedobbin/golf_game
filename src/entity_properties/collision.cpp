#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <assert.h>
#include <algorithm>
#include "../entities/item.hpp"
#include "../entities/living_entity.hpp"
#include "../world.hpp"

//circ dep
#include "../entities/entity.hpp"

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

bool Collision::isValidPos(rect pos)
{
    if (!solid){
        return true;
    }

    for (auto& e : World::activeLevel->entities){
        if (e.get() == owner){
            continue;
        }
        if (!e->collision){
            continue;
        }
        if (e->collision->isNotOrSemiSolid() && isNotOrSemiSolid()){
            continue;
        }    

        if (intersectCollides(getIntersect(pos, e->pos))){
            return false;
        }
    }
    return true;
}

void Collision::pushout(Entity* collider, direction colliderDir, rect intersect)
{
    if (!collider->collision){
        return;
    }

    if (isNotOrSemiSolid() && collider->collision->isNotOrSemiSolid()){
        return;
    }

    // dont mess with entities in golfmode
    if (collider->type == LIVING && ((LivingEntity*)collider)->golfMode && ((LivingEntity*)collider)->golfMode->active){
        return;
    }

    rect ownerRect = owner->pos;
    rect colliderRect = collider->pos;

    bool bothSolid = collider->collision->solid && owner->collision->solid;

    //allow going up slopes
    if (colliderDir == LEFT || colliderDir == RIGHT){
        if ((collider->pos.y + collider->pos.h) - owner->pos.y <= collider->pos.h / 10){//TODO: get rid of magic number
            return;
        }
    }

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
    if (collider->behavior && collider->behavior->destroyed){
        return false;
    }

    if (owner->type == LIVING){
        auto livingOwner = (LivingEntity*) owner;
        auto ignored = livingOwner->ignoreEffectsList;
        if (std::find_if(ignored.begin(), ignored.end(), [collider](auto entity){
            return entity == collider;
        }) != ignored.end()){
            std::cout << "DEBUG: "<< owner->name << " ignored effects of " << collider->name << std::endl;
            return false;
        }

        switch(collider->type){
            case ITEM: {
                //if not owned by an entity, its on the field
                auto item = (Item*)collider;
                if (!item->owner && owner->behavior->pickupItems){
                    auto living = (LivingEntity*) owner;
                    living->give(item);
                }
                break;
            }
            case LIVING: {
                if (owner->behavior){
                    if ( ((LivingEntity*)collider)->behavior->type == ENEMY_A){
                        owner->behavior->destroy();
                    }
                }
                break;
            }
            case BALL: {
                auto ball = collider;
                
                const int xKillSpeed = 5;
                const int yKillSpeed = 2;

                if (ball->behavior->xSpeed > xKillSpeed && colliderDir == RIGHT){
                    owner->behavior->destroy();
                } else if (ball->behavior->xSpeed < xKillSpeed && colliderDir == LEFT){
                    owner->behavior->destroy();
                }

                if (ball->behavior->ySpeed > yKillSpeed && colliderDir == DOWN){
                    owner->behavior->destroy();
                } else if (ball->behavior->ySpeed < yKillSpeed && colliderDir == UP){
                    owner->behavior->destroy();
                }

                break;
            }
            case SPIKES :{
                std::cout << "DEBUG: TODO: spikes effect" << std::endl;
                break;
            }
            default:
                break;//Get rid of clang warning because not all cases are handled, but that is fine..
        }
    }
    return false;
}

bool Collision::isNotOrSemiSolid()
{
    if (!solid){
        return true;
    }
    if (owner->type == LIVING){
        return true;
    }
    if (owner->type == ITEM){
        return true;
    }
    if (owner->type == BALL){
	//if (owner->type == BALL && (owner->behavior->xSpeed == 0 && owner->behavior->ySpeed == 0)){
        return true;
    }
    return false;
}
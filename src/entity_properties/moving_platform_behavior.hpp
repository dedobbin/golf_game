#pragma once

#include "behavior.hpp"
#include "../entities/entity.hpp"
#include "../utils/rect.hpp"
#include "../utils/direction.hpp"

class MovingPlatformBehavior : public Behavior
{
	public: 
		MovingPlatformBehavior(Entity* owner, rect endPos, float speed);
		void behave();
        float speed;
        rect endPos;
    private:
        rect startPos;
        rect curDestination;
        direction curYDir = NONE;
        direction curXDir = NONE;
};
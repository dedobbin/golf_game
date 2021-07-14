#pragma once

#include "behavior.hpp"
#include "../entities/entity.hpp"
#include "../utils/rect.hpp"
#include "../utils/direction.hpp"

class MovingPlatformBehavior : public Behavior
{
	public: 
		MovingPlatformBehavior(Entity* owner, rect endPos);
		void behave();
        /* acc platform will go on when it moves*/
        float potentionalAcc;
        rect endPos;
    private:
        rect startPos;
        rect curDestination;
        direction curYDir = NONE;
        direction curXDir = NONE;
};
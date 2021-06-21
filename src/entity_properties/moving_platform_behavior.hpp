#pragma once

#include "behavior.hpp"
#include "../entities/entity.hpp"
#include "../utils/rect.hpp"
#include "../utils/direction.hpp"

class MovingPlatformBehavior : public Behavior
{
	public: 
		MovingPlatformBehavior(Entity* owner, rect endPos, int speed);
		void behave();
        int speed;
        rect endPos;
    private:
        direction curDir = direction::NONE;
};
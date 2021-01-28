#pragma once

#include "direction.hpp"

class LivingEntity;
class Entity;

enum GolfState 
{
	AIMING_POWER,
	AIMING_HEIGHT,
	SWINGING, 
};

class GolfMode 
{
	public:
		GolfMode(LivingEntity* owner);
		void activate(Entity* ball);
		bool active = false;
		void setDirection(direction dir);
		direction _dir;
		GolfState state;
		int power = 0;
		int height = 0;
		int nPoints = 25;
		//Used to delay speed of cursor
		int ticksAfterLastMeterMove = 0;
		direction meterCursorDirection = RIGHT;
		void tick();
	private:
		Entity* _ball;
		Entity* owner;
};

#pragma once

#include "../utils/direction.hpp"

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
		direction _dir = RIGHT;
		GolfState state;
		//Used to delay speed of cursor
		int ticksAfterLastMeterMove = 0;
		direction meterCursorDirection = LEFT;
		int ticksAfterStartSwinging = 0;
		void tick();
		Entity* _ball = 0;

		/** graphic stuff  **/
		int powerCursor = 0;
		int heightCursor = 0;
		int nPoints = 25;

	private:
		void reset();
		void shoot();
		Entity* owner;
};

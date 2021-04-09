#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "../utils/rect.hpp"
#include <memory>

/* circ dep, could get rid of this by doing entity->followWithCam(camera) */
class Entity;

class Camera
{
	public:
		Camera(int x, int y, int w, int h);
		bool inView(int x, int y, int w, int h);
		bool partiallyInView(int x, int y, int w, int h);
		void followWithCam(std::shared_ptr<Entity> entity, bool snapIfOutsideOfView = true);
		void snapToSanePos(std::shared_ptr <Entity> entity);

		rect camRect;
};


#endif
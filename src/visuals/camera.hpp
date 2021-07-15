#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "../utils/rect.hpp"
#include <memory>

/* circ dep, could get rid of this by doing entity->followWithCam(camera) */
class Entity;

enum CameraType {
	NO_FOLLOW,
	FOLLOW,
	CENTER
};
class Camera
{
	public:
		Camera(int x, int y, int w, int h, CameraType type = CameraType::FOLLOW);
		bool inView(int x, int y, int w, int h);
		bool partiallyInView(int x, int y, int w, int h);
		void followWithCam(std::shared_ptr<Entity> entity);
		void snapToSanePos(std::shared_ptr <Entity> entity);

		CameraType type;
		rect camRect;
};


#endif
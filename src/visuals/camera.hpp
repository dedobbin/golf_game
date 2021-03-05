#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "../utils/rect.hpp"

class Camera
{
	public:
		Camera(int x, int y, int w, int h);
		bool inView(int x, int y, int w, int h);
		rect camRect;
};


#endif
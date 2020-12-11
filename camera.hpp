#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "rect.hpp"

class Camera
{
	public:
		Camera(int x, int y, int w, int h);
		bool inView(int x, int y, int w, int h);
	private:
		rect camRect;
};


#endif
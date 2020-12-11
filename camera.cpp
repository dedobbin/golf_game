#include "camera.hpp"

Camera::Camera(int x, int y, int w, int h)
:camRect({x, y, w, h})
{}

bool Camera::inView(int x, int y, int w, int h)
{
	return true;
}
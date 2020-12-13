#include "animation.hpp"
#include <iostream>

Frame::Frame(int x, int y, int w, int h)
: src({x, y, w, h})
{}

Frame::~Frame()
{
	std::cout << "DEBUG: Frame deconstructor" << std::endl;
}

Animation::~Animation()
{
	std::cout << "DEBUG: Animation deconstructor" << std::endl;
}
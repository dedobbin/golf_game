#include "game.hpp"
#include <memory>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
extern "C" {
	EMSCRIPTEN_KEEPALIVE
	void start_level(int input, int len)
	{
		std::cout << "DEBUG: yay" << std::endl;
		auto g = std::make_unique<Game>();
		g->setupWorld();
		g->start();
	}
}

#elif

int main(int argc, char* argv[])
{
	//TODO: load level from arguments
	auto g = std::make_unique<Game>();
	g->setupWorld();
	std::cout << "DEBUG: game starts" << std::endl;
	g->start();
	std::cout << "DEBUG: game ends" << std::endl;
	return 0;
}
#endif


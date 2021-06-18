#include "game.hpp"
#include <memory>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <string>
#include "utils/json.hpp"

extern "C" {
	EMSCRIPTEN_KEEPALIVE
	bool start_level(char* input)
	{
		//std::string s(input);
		//std::cout << "DEBUG input: " << input <<std::endl;  
		auto g = std::make_unique<Game>();
		g->setupWorld();
		std::cout << "DEBUG: game starts" << std::endl;
		g->start();
		return true;
	}
}

#else

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


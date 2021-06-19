#include "game.hpp"
#include <memory>
#include <iostream>
#include <sstream>

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
		
		std::string s(input);
		nlohmann::json j;
		std::stringstream (s)>>j;
		auto g = std::make_unique<Game>();
		g->setupWorld(j);
		std::cout << "DEBUG: game starts" << std::endl;
		g->start();
		return true;
	}
}

#else

int main(int argc, char* argv[])
{
	std::string levelPath;

	if (argc == 1){
		levelPath = "resources/levels/2.json";
	} else {
		levelPath = argv[1];
	}

	auto g = std::make_unique<Game>();
	if (!g->setupWorld(levelPath)){
		std::cout << "Couldn't open (level) file: " << levelPath << std::endl;
		return 1;
	}
	std::cout << "DEBUG: game starts" << std::endl;
	g->start();
	std::cout << "DEBUG: game ends" << std::endl;
	return 0;
}
#endif


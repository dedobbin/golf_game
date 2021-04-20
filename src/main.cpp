#include "game.hpp"
#include <memory>
#include <iostream>

int main(int argc, char* argv[])
{
	auto g = std::make_unique<Game>();
	g->setupWorld();
	std::cout << "DEBUG: game starts" << std::endl;
	g->start();
	std::cout << "DEBUG: game ends" << std::endl;
	return 0;
}


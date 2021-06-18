#pragma once
#include <memory.h>
#include "utils/sdl_utils.hpp"
#include "visuals/visuals.hpp"

enum HandleInputReturnType{
	CONTINUE,
	QUIT,
	RESET
};

class Game 
{
	public:
		Game();
		void setupWorld(); 		//TODO: use dynamic input source
		void start();
		std::unique_ptr<Visuals> visuals; //public because emscriptenLoop needs access to it.. 
		// Returns false when game ends, also public because emscriptenLoop needs access to it..
		bool tick();

	private:
		void renderEverything();
		HandleInputReturnType handleInput();
		int ticksAfterPlayedDied = 0;

		std::shared_ptr<LivingEntity> player;

		std::shared_ptr<Entity> followWithCam;

		bool keysPressed[322] = {false};

		int fpsTextIndex = -1;
		LazyFooTimer fpsTimer;
#ifndef __EMSCRIPTEN__
		LazyFooTimer capTimer;
#endif
};
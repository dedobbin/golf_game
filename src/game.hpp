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
		//TODO: use dynamic input source
		void setupWorld();
		void start();
	private:
		void renderEverything();
		HandleInputReturnType handleInput();
		// Returns false when game ends
		bool tick();
#ifdef __EMSCRIPTEN__
		void emscriptenLoop(void *arg)
#else 
		void startGameNative();
		LazyFooTimer capTimer;
#endif
		std::unique_ptr<Visuals> visuals;
		int ticksAfterPlayedDied = 0;

		std::shared_ptr<LivingEntity> player;

		std::shared_ptr<Entity> followWithCam;

		bool keysPressed[322] = {false};

		int fpsTextIndex = -1;
		LazyFooTimer fpsTimer;
};
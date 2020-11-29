#include <iostream>
#include "visuals.hpp"
#include "sdl_utils.hpp"

LazyFooTimer fpsTimer;
LazyFooTimer capTimer;

int main (int argc, char* argv[])
{
	Visuals v;
	auto sheet = v.getSpritesheet("spritesheet1");

	std::vector<Sprite*> sprites = {
		new Sprite(sheet, {0, 0, 32, 32}, {0, 0, 100, 100})
	};

	for (auto s : sprites){
		v.addSprite(s);
	}
	
	int countedFrames = 0;
	const int FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / FPS;

	bool keepGoing = true;

	fpsTimer.start();
	
	while(keepGoing){
		capTimer.start();

		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 ){
			if (e.type == SDL_QUIT){
				keepGoing = false;
			} else if (e.type == SDL_KEYDOWN){
		
			} else {
		
			}
		}

		v.render();

		float avgFps = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if( avgFps > 2000000 ){
			avgFps = 0;
		}
		//std::cout << "FPS: " << avgFps << std::endl;
		++countedFrames;
		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICK_PER_FRAME ){
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}
	}

	std::cout << "DEBUG: game end" << std::endl;

	for (auto s : sprites){
		delete(s);
	}
	
	return 0;
}


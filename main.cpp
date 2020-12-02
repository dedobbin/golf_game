#include <iostream>
#include <memory>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include <vector>

LazyFooTimer fpsTimer;
LazyFooTimer capTimer;

int main (int argc, char* argv[])
{
	Visuals v;
	// Get all spritesheets
	auto sheet1 = v.getSpritesheet("spritesheet1");
	auto sheet2 = v.getSpritesheet("spritesheet2");

	// Setup entities
	std::vector<std::unique_ptr<Entity>> entities;

	auto e = std::make_unique<Entity>();
	e->sprite = std::unique_ptr<Sprite>(new Sprite(sheet1, {0, 0, 32, 32}, e.get()));
	e->behavior = std::unique_ptr<Behavior>(new Behavior(e.get()));
	Entity* player = e.get();

	entities.push_back(std::move(e));
	
	// Get to actual game loop stuff
	int countedFrames = 0;
	const int FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / FPS;

	bool keepGoing = true;

	fpsTimer.start();
	
	bool keysPressed[322] = {false};

	while(keepGoing){
		capTimer.start();

		// Gather inputs and apply to player behavior
		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 ){
			if (e.type == SDL_QUIT){
				keepGoing = false;
			} else if (e.type == SDL_KEYDOWN){
				keysPressed[e.key.keysym.scancode] = true;
			} else  if (e.type == SDL_KEYUP){
				keysPressed[e.key.keysym.scancode] = false;
			}
		}
		if (keysPressed[SDL_SCANCODE_RIGHT]){
			player->behavior->addXSpeed(player->behavior->walkAcc);
		} else if (player->behavior->xSpeed > 0){
			player->behavior->addXSpeed(-0.1);
		}
		if (keysPressed[SDL_SCANCODE_LEFT]){
			player->behavior->addXSpeed(-player->behavior->walkAcc);
		} else if (player->behavior->xSpeed < 0){
			player->behavior->addXSpeed(0.1);
		}


		for (auto& e : entities){
			if (e->behavior){
				e->behavior->behave();
			}
		}
		
		v.renderStart();
		for (auto& e : entities){
			if (e->sprite){
				v.renderSprite(e->sprite.get());
			}
		}
		v.renderEnd();

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


	// Visuals v;
	// auto sheet1 = v.getSpritesheet("spritesheet1");
	// auto sheet2 = v.getSpritesheet("spritesheet2");

	// std::vector<Entity*> entities;

	// entities.push_back(new Entity(new Sprite(sheet1, {0, 0, 32, 32}, {0, 0, 100, 100})));

	// int i = 0;
	// int w = 32;
	// for (int i = 0; i < SCREEN_W; i += w){
	// 	entities.push_back(new Entity(new Sprite(sheet2, {0, 0, 32, 32}, {i, 300, 32, 32})));
	// }

	// for (auto e : entities){
	// 	v.addSprite(e->sprite);
	// }
	
	// int countedFrames = 0;
	// const int FPS = 60;
	// const int SCREEN_TICK_PER_FRAME = 1000 / FPS;

	// bool keepGoing = true;

	// fpsTimer.start();
	
	// while(keepGoing){
	// 	capTimer.start();

	// 	SDL_Event e;
	// 	while( SDL_PollEvent( &e ) != 0 ){
	// 		if (e.type == SDL_QUIT){
	// 			keepGoing = false;
	// 		} else if (e.type == SDL_KEYDOWN){
		
	// 		} else {
		
	// 		}
	// 	}

	// 	v.render();

	// 	float avgFps = countedFrames / ( fpsTimer.getTicks() / 1000.f );
	// 	if( avgFps > 2000000 ){
	// 		avgFps = 0;
	// 	}
	// 	//std::cout << "FPS: " << avgFps << std::endl;
	// 	++countedFrames;
	// 	int frameTicks = capTimer.getTicks();
	// 	if( frameTicks < SCREEN_TICK_PER_FRAME ){
	// 		//Wait remaining time
	// 		SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
	// 	}
	// }

	// std::cout << "DEBUG: game end" << std::endl;

	// for (auto e : entities){
	// 	delete(e);
	// }
	
	return 0;
}


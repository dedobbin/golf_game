#include <iostream>
#include <memory>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include <vector>

LazyFooTimer fpsTimer;
LazyFooTimer capTimer;

#define DEBUG_CONTROLS

int main (int argc, char* argv[])
{
	Visuals v;
	// Get all spritesheets
	auto sheet1 = v.getSpritesheet("spritesheet1");
	auto sheet2 = v.getSpritesheet("spritesheet2");

	// Setup entities
	std::vector<std::unique_ptr<Entity>> entities;

	auto e = std::make_unique<Entity>("player", 200, 0, 100, 100);
	e->sprite = std::unique_ptr<Sprite>(new Sprite(sheet1, {0, 0, 32, 32}, e.get()));
	e->behavior = std::unique_ptr<Behavior>(new Behavior(e.get()));
#ifdef DEBUG_CONTROLS
	e->behavior->gravity = false;
#endif
	e->collision = std::unique_ptr<Collision>(new Collision(e.get(), true));
	Entity* player = e.get();

	entities.push_back(std::move(e));

	int blockW = 100;
	int blockH = 100;
	int i = 0;
	for (i = 0; i < 10; i++){
		auto b = std::make_unique<Entity>("block" + std::to_string(i), i * blockW, 300, blockW, blockH);
		b->sprite = std::unique_ptr<Sprite>(new Sprite(sheet2, {0, 0, 32, 32}, b.get()));
		b->collision = std::unique_ptr<Collision>(new Collision(b.get(), true));
		entities.push_back(std::move(b));
	}

	auto b = std::make_unique<Entity>("block" + std::to_string(++i), 700, 200, 100, 100);
	b->sprite = std::unique_ptr<Sprite>(new Sprite(sheet2, {0, 0, 32, 32}, b.get()));
	b->collision = std::unique_ptr<Collision>(new Collision(b.get(), true));
	entities.push_back(std::move(b));

	auto b2 = std::make_unique<Entity>("block" + std::to_string(++i), 700, 0, 100, 100);
	b2->sprite = std::unique_ptr<Sprite>(new Sprite(sheet2, {0, 0, 32, 32}, b2.get()));
	b2->collision = std::unique_ptr<Collision>(new Collision(b2.get(), true));
	entities.push_back(std::move(b2));

	auto b3 = std::make_unique<Entity>("block" + std::to_string(++i), 0, 200, 100, 100);
	b3->sprite = std::unique_ptr<Sprite>(new Sprite(sheet2, {0, 0, 32, 32}, b3.get()));
	b3->collision = std::unique_ptr<Collision>(new Collision(b3.get(), true));
	entities.push_back(std::move(b3));

	auto b4 = std::make_unique<Entity>("block" + std::to_string(++i), 0, 0, 100, 100);
	b4->sprite = std::unique_ptr<Sprite>(new Sprite(sheet2, {0, 0, 32, 32}, b4.get()));
	b4->collision = std::unique_ptr<Collision>(new Collision(b4.get(), true));
	entities.push_back(std::move(b4));

	for (auto& e : entities){
		std::cout << e->debugString() << std::endl;
	}

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

#ifdef DEBUG_CONTROLS
		if (keysPressed[SDL_SCANCODE_DOWN]){
			player->behavior->addYSpeed(player->behavior->walkAcc);
		} else if (player->behavior->ySpeed > 0){
			player->behavior->addYSpeed(-0.1);
		}
		if (keysPressed[SDL_SCANCODE_UP]){
			player->behavior->addYSpeed(-player->behavior->walkAcc);
		} else if (player->behavior->ySpeed < 0){
			player->behavior->addYSpeed(0.1);
		}
#endif

		

		//Move etc all entities, collision etc
		for (auto& e : entities){

			if (e->behavior){
				e->behavior->behave();
			}

			//Collision
			for (auto& collidee : entities){
				if (e == collidee) continue;
				Collision::checkCollision(e.get(), collidee.get());
			}
		}
		
		// Render everything
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


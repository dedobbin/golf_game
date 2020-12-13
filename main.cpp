#include <iostream>
#include <memory>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include "rect.hpp"

#include <vector>

LazyFooTimer fpsTimer;
LazyFooTimer capTimer;
Visuals v;
Entity* player = NULL;
std::vector<std::unique_ptr<Entity>> entities;

#define DEBUG_DRAW
//#define DEBUG_CAMERA

void generateEntities()
{
	// Get all spritesheets
	auto sheet1 = v.getSpritesheet("spritesheet1");
	auto sheet2 = v.getSpritesheet("spritesheet2");
	auto sheet3 = v.getSpritesheet("spritesheet3");

	// Setup entities
	Entity* e = new Entity("player", 200, 0, 70, 100);
	e->sprite = std::unique_ptr<Sprite>(new Sprite(e));
	e->behavior = std::unique_ptr<Behavior>(new Behavior(e));
	//e->behavior->gravity = false;
	e->collision = std::unique_ptr<Collision>(new Collision(e, true));
	player = e;
	entities.emplace_back(std::unique_ptr<Entity>(e));

	int blockW = 100;
	int blockH = 100;
	for (int i = 0; i < 10; i++){
		Entity* b = new Entity("block" + std::to_string(i), i * blockW, 400, blockW, blockH);
		b->sprite = std::unique_ptr<Sprite>(new Sprite(b));
		b->collision = std::unique_ptr<Collision>(new Collision(b, true));
		entities.emplace_back(std::unique_ptr<Entity>(b));
		for (int j = 0; j < i ; j++){
			Entity* bv = new Entity("block" + std::to_string(i), i * blockW, 300 - (blockH/5) * j, blockW, blockH/5);
			bv->sprite = std::unique_ptr<Sprite>(new Sprite(bv));
			bv->collision = std::unique_ptr<Collision>(new Collision(bv, true));
			entities.emplace_back(std::unique_ptr<Entity>(bv));
		}
	}
}
int main (int argc, char* argv[])
{
	generateEntities();
	int countedFrames = 0;
	const int FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / FPS;

	bool keepGoing = true;

	fpsTimer.start();
	
	bool keysPressed[322] = {false};

	while(keepGoing){
		capTimer.start();

#ifndef DEBUG_CAMERA
		v.camera->camRect.x = player->x - v.camera->camRect.w / 2;
		v.camera->camRect.y = player->y - v.camera->camRect.h / 2;
#endif

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
		#define SLOW_DOWN_AMOUNT 0.2
		if (keysPressed[SDL_SCANCODE_RIGHT]){
			player->behavior->addXSpeed(player->behavior->walkAcc);
		} else if (player->behavior->xSpeed > 0){
			player->behavior->addXSpeed(-SLOW_DOWN_AMOUNT, true);
		}
		if (keysPressed[SDL_SCANCODE_LEFT]){
			player->behavior->addXSpeed(-player->behavior->walkAcc);
		} else if (player->behavior->xSpeed < 0){
			player->behavior->addXSpeed(SLOW_DOWN_AMOUNT, true);
		}
		if (keysPressed[SDL_SCANCODE_UP]){
			player->behavior->jump();
		}

#ifdef DEBUG_CAMERA
		int camSpeed = 5;
		if (keysPressed[SDL_SCANCODE_D]){
			v.camera->camRect.x += camSpeed;
		} 
		if (keysPressed[SDL_SCANCODE_A]){
			v.camera->camRect.x -= camSpeed;
		} 
		if (keysPressed[SDL_SCANCODE_W]){
			v.camera->camRect.y -= camSpeed;
		} 
		if (keysPressed[SDL_SCANCODE_S]){
			v.camera->camRect.y += camSpeed;
		} 
#endif

		//Move etc all entities, collision etc
		for (auto& e : entities){
			if (e->behavior){
				e->behavior->behave();
			}

			//Collision
			bool hasGroundUnderneath = false;
			rect groundIntersect = {0, 0, 0, 0};

			for (auto& collidee : entities){
				if (e == collidee) continue;
				
				auto intersect = Collision::checkCollision(e.get(), collidee.get());
				bool collision = intersect.w > 0 && intersect.h > 0;

				if (collision){
					std::cout << "DEBUG: collision: " << e->name << " and " << collidee->name << std::endl;
					collidee->collision->effect(e.get(), intersect);
				}

				// Check if ground underneath so flip grounded to false 
				// TODO: would be nice if isolated 'pushout' collision from Collision::effect and also used it here
				if (e->behavior && collidee->collision->solid && e->collision->solid){
					int originalY = e->y;

					e->y += 1;
					auto intersect = Collision::checkCollision(e.get(), collidee.get());
					bool collision = intersect.w > 0 && intersect.h > 0;
					if (collision){
						std::cout << "DEBUG: has ground underneath: " << e->name << " and " << collidee->name << std::endl;
						hasGroundUnderneath = true;
						groundIntersect = intersect;
					}

					e->y = originalY;
				}
			}

			if (!hasGroundUnderneath && e->behavior && e->behavior->gravity){
				e->behavior->grounded = false;
			} else if (hasGroundUnderneath && e->behavior && e->behavior->gravity && e->behavior->ySpeed > 0){
				e->behavior->grounded = true;
				e->behavior->ySpeed = 0;
                e->y -= groundIntersect.h;
			}
		}
		
		// Render everything
		v.renderStart();
		for (auto& e : entities){
			if (e->sprite){
				v.renderEntity(e.get());
#ifdef DEBUG_DRAW 
				if (e->collision){
					rect collisionRect = e->collision->getRect();
					v.renderRect(collisionRect.x, collisionRect.y, collisionRect.w, collisionRect.h);
				}
#endif	
			}
		}
#ifdef DEBUG_DRAW 
	v.renderRectOverlay(0, 0, v.camera->camRect.w, v.camera->camRect.w);
#endif	

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


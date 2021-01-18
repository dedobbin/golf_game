#include <iostream>
#include <memory>
#include <algorithm>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include "living_entity.hpp"
#include "rect.hpp"
#include "animated_graphic.hpp"
#include "entity_factory.hpp"
#include <assert.h>
#include <vector>
#include "direction.hpp"
#include "item_behavior.hpp"


// #define DEBUG_DRAW
// #define DEBUG_CAMERA
// #define DEBUG_CONTROLS

//TODO: get rid of globals so functions can go to own files etc
LazyFooTimer fpsTimer;
LazyFooTimer capTimer;
Visuals v;
std::shared_ptr<LivingEntity> player;
std::vector<std::shared_ptr<Entity>> entities;

std::shared_ptr<Entity> followWithCam = NULL;

rect playerStartPos = {200, 0};

bool keysPressed[322] = {false};

void generateEntities(std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	EntityFactory factory(spritesheets);

	entities.emplace_back(factory.createGolfClub(400, 0));

	entities.emplace_back(factory.createPlayer(200, 0));
	player = std::static_pointer_cast<LivingEntity>(entities.back());

	entities.emplace_back(factory.createEnemy(300, 0));

	entities.emplace_back(factory.createBall(300, 0));

	int blockW = 100;
	int blockH = 100;
	int i = 0;
	entities.emplace_back(factory.createBlock(i * blockW, 400 - blockH, blockW, blockH));
	
	for (i = 0; i < 15; i++){
		entities.emplace_back(factory.createBlock(i * blockW, 400, blockW, blockH));
		if (i > 6){
			if (i < 9){
				entities.emplace_back(factory.createBlock(i * blockW, 200, blockW, blockH));
			}else {
				entities.emplace_back(factory.createBlock(i * blockW, 100, blockW, blockH));
			}
		}
		// for (int j = 0; j < i ; j++){
		// 	entities.emplace_back(factory.createBlock(i * blockW, 300 - (blockH/5) * j, blockW, blockH/5));
		// }
	}
	entities.emplace_back(factory.createBlock(i * blockW - blockW, 400 - blockH, blockW, blockH));
	
}

/* Returns false if quit */
bool handleInput()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ){
		if (e.type == SDL_QUIT){
			return false;
		} else if (e.type == SDL_KEYDOWN){
			keysPressed[e.key.keysym.scancode] = true;
		} else  if (e.type == SDL_KEYUP){
			keysPressed[e.key.keysym.scancode] = false;
		}
	}

	if (keysPressed[SDL_SCANCODE_RIGHT]){
		player->behavior->xPush = RIGHT;
	} else if (player->behavior->xSpeed > 0){
		player->behavior->xPush = NONE;
	}

	if (keysPressed[SDL_SCANCODE_LEFT]){
		player->behavior->xPush = LEFT;
	} else if (player->behavior->xSpeed < 0){
		player->behavior->xPush = NONE;
	}

	if (keysPressed[SDL_SCANCODE_Z]){
		player->behavior->jump();
	}

	if (keysPressed[SDL_SCANCODE_C]){
		auto arr = player->behavior->surroundings.inside;
		auto i = std::find_if(arr.begin(),
			arr.end(), 
			[&](const auto& val){ return val->type == BALL; } );
		if (i != arr.end()){
			if (player->golfMode){
				player->golfMode = false;
			} else if (player->heldItem){
				auto itemBehavior = (ItemBehavior*)player->heldItem->behavior.get();
				itemBehavior->interact(*i);
			}
		}
	}

#ifdef DEBUG_CONTROLS
	if (keysPressed[SDL_SCANCODE_BACKSPACE]){
		player->pos.x = playerStartPos.x;
		player->pos.y = playerStartPos.y;
		player->behavior->xSpeed = 0;
		player->behavior->ySpeed = 0;
	}
#endif

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

	return true;
}

void renderEverything()
{
			v.renderStart();
		for (auto& e : entities){
			if (e->type == ITEM){
				int d = 4;
			}
			if (e->graphic){
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
}

int main (int argc, char* argv[])
{
	assert(v.spritesheets.size() > 0);
	generateEntities(v.spritesheets);
	
	assert(player);
	followWithCam = player;
	
	
	int countedFrames = 0;
	const int FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / FPS;

	bool keepGoing = true;

	fpsTimer.start();

	while(keepGoing){
		capTimer.start();

		keepGoing = handleInput();

		//Move etc all entities, collision etc
		for (auto& e : entities){
			if (e->behavior){
				e->behavior->behave(entities);
			}
		}

#ifndef DEBUG_CAMERA
		v.camera->camRect.x = followWithCam->pos.x - v.camera->camRect.w / 2;
		v.camera->camRect.y = followWithCam->pos.y - v.camera->camRect.h / 2;
#endif

		renderEverything();

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
	return 0;
}


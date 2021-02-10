#include <iostream>
#include <memory>
#include <algorithm>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include "living_entity.hpp"
#include "rect.hpp"
#include "entity_factory.hpp"
#include <assert.h>
#include <vector>
#include "direction.hpp"
#include "item_behavior.hpp"
#include "world.hpp"

#define DEBUG_DRAW
// #define DEBUG_CAMERA
// #define DEBUG_CONTROLS

//TODO: get rid of globals so functions can go to own files etc
LazyFooTimer fpsTimer;
LazyFooTimer capTimer;
Visuals v;
std::shared_ptr<LivingEntity> player;

std::shared_ptr<Entity> followWithCam = NULL;

rect playerStartPos = {200, 0};

bool keysPressed[322] = {false};

void generateEntities(std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	//TODO: get from file
	EntityFactory factory(spritesheets);

	World::entities.emplace_back(factory.createGolfClub(300, 0));

	World::entities.emplace_back(factory.createPlayer(100, 0));
	player = std::static_pointer_cast<LivingEntity>(World::entities.back());

	World::entities.emplace_back(factory.createEnemy(300, 0));

	World::entities.emplace_back(factory.createBall(100, 0));

	int blockW = 100;
	int blockH = 100;
	int i = 0;
	World::entities.emplace_back(factory.createBlock(i * blockW, 400 - blockH, blockW, blockH));
	
	for (i = 0; i < 20; i++){
		World::entities.emplace_back(factory.createBlock(i * blockW, 400, blockW, blockH));
		if (i > 6){
			if (i < 9){
				World::entities.emplace_back(factory.createBlock(i * blockW, 200, blockW, blockH));
			}else {
				World::entities.emplace_back(factory.createBlock(i * blockW, 100, blockW, blockH));
			}
		}
		// for (int j = 0; j < i ; j++){
		// 	World::entities.emplace_back(factory.createBlock(i * blockW, 300 - (blockH/5) * j, blockW, blockH/5));
		// }
	}
	World::entities.emplace_back(factory.createBlock(i * blockW - blockW, 400 - blockH, blockW, blockH));
}

/* Returns false if quit */
bool handleInput()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ){
		if (e.type == SDL_QUIT){
			return false;
		} else if (e.type == SDL_KEYDOWN){
			
			if (player->golfMode->active){
				if (player->golfMode->state == AIMING_POWER){
					if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT){
						player->golfMode->setDirection(RIGHT);
					}
					if (e.key.keysym.scancode == SDL_SCANCODE_LEFT){
						player->golfMode->setDirection(LEFT);
					}
					
					if (e.key.keysym.scancode == SDL_SCANCODE_C){
						player->golfMode->state = AIMING_HEIGHT;
					}
				} else if (player->golfMode->state == AIMING_HEIGHT){
					if (e.key.keysym.scancode == SDL_SCANCODE_C){
						player->golfMode->state = SWINGING;
					}
				} 
			} else {
				if (e.key.keysym.scancode == SDL_SCANCODE_C){
					auto i = std::find_if( player->collision->currentColliders.begin(),
					player->collision->currentColliders.end(), 
					[&](const auto val){ return val->type == BALL && val->behavior && val->behavior->grounded; } 
					);

					if (i != player->collision->currentColliders.end()){
						if (player->heldItem){
							auto itemBehavior = (ItemBehavior*)player->heldItem->behavior.get();
							itemBehavior->interact(*i);
						}
					}
				}
			}

			keysPressed[e.key.keysym.scancode] = true;
		} else if (e.type == SDL_KEYUP){
			keysPressed[e.key.keysym.scancode] = false;
		}
	}

	if (!player->golfMode->active){
		if (keysPressed[SDL_SCANCODE_RIGHT]){
			player->xPush = RIGHT;
		} else if (player->behavior->xSpeed > 0){
			player->xPush = NONE;
		}

		if (keysPressed[SDL_SCANCODE_LEFT]){
			player->xPush = LEFT;
		} else if (player->behavior->xSpeed < 0){
			player->xPush = NONE;
		}

		if (keysPressed[SDL_SCANCODE_Z]){
			player->behavior->jump();
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
	for (auto& e : World::entities){
		if (e->type == ITEM){
			int d = 4;
		}
		if (e->graphic){
			v.renderEntity(e.get());

#ifdef DEBUG_DRAW 
			if (e->collision){
				v.renderRect(e->pos.x, e->pos.y, e->pos.w, e->pos.h);
			}
#endif	
		}
	}
#ifdef DEBUG_DRAW 
	v.renderRectOverlay(0, 0, v.camera->camRect.w, v.camera->camRect.w);
#endif	
		
	if (player->golfMode->active){
		if (player->golfMode->state == AIMING_POWER){
			v.renderGolfMeter(AIMING_POWER, player->golfMode->powerCursor, player->golfMode->nPoints);
		} else if (player->golfMode->state == AIMING_HEIGHT){
			v.renderGolfMeter(AIMING_HEIGHT, player->golfMode->heightCursor, player->golfMode->nPoints);
		}
	}

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
		for (auto& e : World::entities){
			if (e->behavior){
				e->behavior->behave();
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


#include <iostream>
#include <memory>
#include <algorithm>
#include "visuals.hpp"
#include "entity.hpp"
#include "living_entity.hpp"
#include "rect.hpp"
#include "entity_factory.hpp"
#include <assert.h>
#include <vector>
#include "direction.hpp"
#include "item_behavior.hpp"
#include "world.hpp"
#include <emscripten.h>

#define DEBUG_DRAW
// #define DEBUG_CAMERA
// #define DEBUG_CONTROLS

//TODO: get rid of globals so functions can go to own files etc
Visuals v;
std::shared_ptr<LivingEntity> player;

std::shared_ptr<Entity> followWithCam = NULL;

rect playerStartPos = {200, 0};

bool keysPressed[322] = {false};

void setupWorld(std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	World::entities = {};
	//TODO: get from file
	EntityFactory factory(spritesheets);

	World::entities.emplace_back(factory.createGolfClub(700, 0));

	World::entities.emplace_back(factory.createEnemy(100, 0));

	World::entities.emplace_back(factory.createPlayer(3000, 0));
	player = std::static_pointer_cast<LivingEntity>(World::entities.back());

	World::entities.emplace_back(factory.createBall(300, 0));

	int x = 0; 
	int y = 300;
	int w = 100;
	int h = 100;
	for (int i = 0; i < 35; i++){
		x = i * w;
		World::entities.emplace_back(factory.createBlock(x, y, w, h));
	}

	h = 10;
	w = 100;
	x = player->pos.x - w * 2;
	y = y - h;
	for (int i =0; i < 5;i ++){
		World::entities.emplace_back(factory.createBlock(x, y, w, h));
		x-= w;
		y-=10;
	}

	h = 30;
	w = 100;
	x = player->pos.x + w * 2;
	y = 300 - h;
	World::entities.emplace_back(factory.createBlock(x, y, w, h));
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

/* Returns false if quit */
bool handleInput()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ){
		if (e.type == SDL_QUIT){
			return false;
		} else if (e.type == SDL_KEYDOWN){
			
			if (player->behavior->destroyed){
				if (e.key.keysym.scancode == SDL_SCANCODE_SPACE){
					std::cout << "DEBUG: restart" << std::endl;
					return false;
				}
			}

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

void mainloop(void *arg)
{
	context *ctx = static_cast<context*>(arg);

	bool keepGoing = handleInput();
	if(!keepGoing){
		setupWorld(v.spritesheets);
		//emscripten_cancel_main_loop();
	}

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

    ctx->iteration++;
}

int main(int argc, char* argv[])
{
	v.loadSpritesheets(v.defaultSpritesheetPath);
	//v.loadSpritesheet("spritesheet1");
	assert(v.spritesheets.size() > 0);
	std::cout << "DEBUG: game starts" << std::endl;
	setupWorld(v.spritesheets);

	assert(player);
	followWithCam = player;

	const int simulate_infinite_loop = 1;
	const int fps = 60; 


	emscripten_set_main_loop_arg(mainloop, &v.ctx, fps, simulate_infinite_loop);
	std::cout << "DEBUG: game ends" << std::endl;

	return 0;
}


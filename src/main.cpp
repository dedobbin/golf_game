#include <iostream>
#include <memory>
#include <algorithm>
#include <assert.h>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "visuals/visuals.hpp"
#include "entities/entity.hpp"
#include "entities/living_entity.hpp"
#include "entity_properties/item_behavior.hpp"
#include "utils/rect.hpp"
#include "utils/direction.hpp"
#include "utils/sdl_utils.hpp"
#include "entity_factory.hpp"
#include "world.hpp"
#include "import_level.hpp"

#define DELAY_BEFORE_GAMEOVER 30 //time between player dying and game over screen popping up
int ticksAfterPlayedDied = 0;

#define DEBUG_DRAW
// #define DEBUG_CAMERA
// #define DEBUG_CONTROLS

//TODO: get rid of globals so functions can go to own files etc
Visuals v;
std::shared_ptr<LivingEntity> player;

std::shared_ptr<Entity> followWithCam = NULL;

rect playerStartPos = {200, 0};

bool keysPressed[322] = {false};

int fpsTextIndex = -1;
LazyFooTimer fpsTimer;

void setupWorld(std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	Import::importLevel(RESOURCES_PATH + std::string("/levels/1.wsp"), spritesheets);
    
	auto it = std::find_if(World::entities.begin(), World::entities.end(), [](const auto& x){
		return x->name == "player";
	});

	std::shared_ptr<LivingEntity> p = std::static_pointer_cast<LivingEntity>(*it);
	player = p;
	followWithCam = player;

	//TODO: get everything from file
	// World::w = 10000000;
	// World::h = 1500;

	// World::entities = {};
	// ticksAfterPlayedDied = 0;

	// EntityFactory factory(spritesheets);

	// World::entities.emplace_back(factory.createGolfClub(700, 0));

	// World::entities.emplace_back(factory.createEnemy(100, 0));

	// World::entities.emplace_back(factory.createPlayer(3000, 0));
	// player = std::static_pointer_cast<LivingEntity>(World::entities.back());

	// World::entities.emplace_back(factory.createBall(300, 0));

	// int x = 0; 
	// int y = 300;
	// int w = 100;
	// int h = 100;
	// for (int i = 0; i < 35; i++){
	// 	x = i * w;
	// 	World::entities.emplace_back(factory.createBlock(x, y, w, h));
	// }

	// h = 10;
	// w = 100;
	// x = player->pos.x - w * 2;
	// y = y - h;
	// for (int i =0; i < 5;i ++){
	// 	World::entities.emplace_back(factory.createBlock(x, y, w, h));
	// 	x-= w;
	// 	y-=10;
	// }

	// h = 30;
	// w = 100;
	// x = player->pos.x + w * 2;
	// y = 300 - h;
	// World::entities.emplace_back(factory.createBlock(x, y, w, h));

	// followWithCam = player;
}

void renderEverything()
{
	v.renderStart();
	for (auto& e : World::entities){
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
	
	if (player->behavior && player->behavior->destroyed){
		ticksAfterPlayedDied ++;
		if (ticksAfterPlayedDied >= DELAY_BEFORE_GAMEOVER){
			v.renderGameOver();
		}
	} else if (player->golfMode && player->golfMode->active){
		if (player->golfMode->state == AIMING_POWER){
			v.renderGolfMeter(AIMING_POWER, player->golfMode->powerCursor, player->golfMode->nPoints);
		} else if (player->golfMode->state == AIMING_HEIGHT){
			v.renderGolfMeter(AIMING_HEIGHT, player->golfMode->heightCursor, player->golfMode->nPoints);
		}
	}

	v.renderTexts();
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
			
			if (player->behavior && player->behavior->destroyed){
				if (e.key.keysym.scancode == SDL_SCANCODE_SPACE){
					v.destroyText(gameOverTextIndex);
					return false;
				}
			}

			if (player->golfMode && player->golfMode->active){
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
					if (player->collision){
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
			}

			keysPressed[e.key.keysym.scancode] = true;
		} else if (e.type == SDL_KEYUP){
			keysPressed[e.key.keysym.scancode] = false;
		}
	}

	if (player->golfMode && !player->golfMode->active){
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

void startGameNative()
{
	while(true){
		float avgFps = v.ctx.iteration / (fpsTimer.getTicks() / 1000.f);
		if (avgFps > 2000000){
			avgFps = 0;
		}
		bool keepGoing = handleInput();
		if (!keepGoing){
			break;
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

		v.ctx.iteration++;

#ifdef DEBUG_DRAW 
		v.updateText(std::to_string(static_cast<int>(avgFps)), fpsTextIndex);
#endif
	//std::cout << "DEBUG: " << player->pos.x << " " << player->pos.y << std::endl;
	//TODO: framecap and early finish
	}
}

void emscriptenLoop(void *arg)
{	
	context *ctx = static_cast<context*>(arg);
	
	float avgFps = ctx->iteration / (fpsTimer.getTicks() / 1000.f);
	if (avgFps > 2000000){
		avgFps = 0;
	}

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

#ifdef DEBUG_DRAW 
	v.updateText(std::to_string(static_cast<int>(avgFps)), fpsTextIndex);
#endif
}

int main(int argc, char* argv[])
{
	fpsTextIndex = v.createText("0", 10, 10);

	v.loadSpritesheets(v.defaultSpritesheetPath);
	//v.loadSpritesheet("spritesheet1");
	
	setupWorld(v.spritesheets);
	
	std::cout << "DEBUG: game starts" << std::endl;

	fpsTimer.start();
#ifdef __EMSCRIPTEN__
	const int simulate_infinite_loop = 1;
	const int fps = -1; //defaults to 60, but setting it to 60 seems to trigger EM_TIMING_SETTIMEOUT, causing frames to drop from 60 when theres actual load?
	emscripten_set_main_loop_arg(emscriptenLoop, &v.ctx, fps, simulate_infinite_loop);
#else
	startGameNative();
#endif

std::cout << "DEBUG: game ends" << std::endl;

	return 0;
}


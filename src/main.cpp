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

const int NATIVE_SCREEN_FPS = 60;
const int NATIVE_SCREEN_TICK_PER_FRAME = 1000 / NATIVE_SCREEN_FPS;

#define DELAY_BEFORE_GAMEOVER 60 //time between player dying and game over screen popping up
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
LazyFooTimer capTimer;

enum HandleInputReturnType{
	CONTINUE,
	QUIT,
	RESET
};

void setupWorld(std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
    World::loadLevel("1.wsp", spritesheets);

	auto it = std::find_if(World::activeLevel->entities.begin(), World::activeLevel->entities.end(), [](const auto& x){
		return x->name == "player";
	});

	std::shared_ptr<LivingEntity> p = std::static_pointer_cast<LivingEntity>(*it);
	player = p;
	followWithCam = player;

	ticksAfterPlayedDied = 0;

	//TODO: get everything from file
	// World::activeLevel->w = 10000000;
	// World::activeLevel->h = 1500;

	// World::activeLevel->entities = {};
	// ticksAfterPlayedDied = 0;

	// EntityFactory factory(spritesheets);

	// World::activeLevel->entities.emplace_back(factory.createGolfClub(700, 0));

	// World::activeLevel->entities.emplace_back(factory.createEnemy(100, 0));

	// World::activeLevel->entities.emplace_back(factory.createPlayer(3000, 0));
	// player = std::static_pointer_cast<LivingEntity>(World::activeLevel->entities.back());

	// World::activeLevel->entities.emplace_back(factory.createBall(300, 0));

	// int x = 0; 
	// int y = 300;
	// int w = 100;
	// int h = 100;
	// for (int i = 0; i < 35; i++){
	// 	x = i * w;
	// 	World::activeLevel->entities.emplace_back(factory.createBlock(x, y, w, h));
	// }

	// h = 10;
	// w = 100;
	// x = player->pos.x - w * 2;
	// y = y - h;
	// for (int i =0; i < 5;i ++){
	// 	World::activeLevel->entities.emplace_back(factory.createBlock(x, y, w, h));
	// 	x-= w;
	// 	y-=10;
	// }

	// h = 30;
	// w = 100;
	// x = player->pos.x + w * 2;
	// y = 300 - h;
	// World::activeLevel->entities.emplace_back(factory.createBlock(x, y, w, h));

	// followWithCam = player;
}

void renderEverything()
{
	v.renderStart();
	for (auto& e : World::activeLevel->entities){
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

HandleInputReturnType handleInput()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ){
		if (e.type == SDL_QUIT){
			return HandleInputReturnType::QUIT;
		} else if (e.type == SDL_KEYDOWN){
			
			if (player->behavior && player->behavior->destroyed){
				if (e.key.keysym.scancode == SDL_SCANCODE_SPACE){
					v.texts[gameOverTextIndex].display = false;
					return HandleInputReturnType::RESET;
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

	return CONTINUE;
}
// Returns false when game ends
bool gameTick()
{
	auto handleInputReturn = handleInput();
	if (handleInputReturn == HandleInputReturnType::RESET){
		setupWorld(v.spritesheets);
	} else if (handleInputReturn == HandleInputReturnType::QUIT){
		return false;
	}

	float avgFps = v.ctx.iteration / (fpsTimer.getTicks() / 1000.f);
	if (avgFps > 2000000){
		avgFps = 0;
	}

	//Move etc all entities, collision etc
	for (auto& e : World::activeLevel->entities){
		if (e->behavior){
			e->behavior->behave();
		}
	}

#ifndef DEBUG_CAMERA
	v.camera->camRect.x = followWithCam->pos.x - v.camera->camRect.w / 2;
	v.camera->camRect.y = followWithCam->pos.y - v.camera->camRect.h / 2;
#endif

	renderEverything();

#ifdef DEBUG_DRAW 
	v.updateText(std::to_string(static_cast<int>(avgFps)), fpsTextIndex);
#endif

	return true;
}

void startGameNative()
{
	bool keepGoing = true;
	while(keepGoing){
		capTimer.start();
		
		keepGoing = gameTick();
		v.ctx.iteration++;

		int frameTicks = capTimer.getTicks();
		if( frameTicks < NATIVE_SCREEN_TICK_PER_FRAME ){
			SDL_Delay( NATIVE_SCREEN_TICK_PER_FRAME - frameTicks );
		}
	}
}


void emscriptenLoop(void *arg)
{	
 	context *ctx = static_cast<context*>(arg);//This is also accessible from v.ctx, so kinda pointless...
	gameTick();
 	ctx->iteration++;
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


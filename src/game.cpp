#include "game.hpp"
#include "world.hpp"
#include "entities/living_entity.hpp"
#include "entity_properties/item_behavior.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

//#define DEBUG_DRAW
//#define DEBUG_CAMERA

#define DELAY_BEFORE_GAMEOVER 60 //time between player dying and game over screen popping up

#ifndef __EMSCRIPTEN__
const int NATIVE_SCREEN_FPS = 60;
const int NATIVE_SCREEN_TICK_PER_FRAME = 1000 / NATIVE_SCREEN_FPS;
#endif

Game::Game()
{
	visuals = std::make_unique<Visuals>();
#ifdef DEBUG_DRAW
	fpsTextIndex = visuals->createText("0", 10, 10);
#endif

#ifdef DEBUG_CAMERA
	visuals->camera->type = CameraType::NO_FOLLOW;
#endif

	//v.loadSpritesheet("spritesheet1");
}

bool Game::setupWorld(std::string path)
{
 	std::ifstream ifs(path);
	nlohmann::json data;

	if (!ifs){
		return false;
	}
	
	ifs >> data;
	setupWorld(data);
	return true;
}

void Game::setupWorld(nlohmann::json data)
{
	//For now, just load all spritesheets
	visuals->loadSpritesheets(visuals->defaultSpritesheetPath);

    World::loadLevel(data, visuals->spritesheets);

	auto it = std::find_if(World::activeLevel->entities.begin(), World::activeLevel->entities.end(), [](const auto& x){
		return x->behavior && x->behavior->type == BehaviorType::PLAYER;
	});

	std::shared_ptr<LivingEntity> p = std::static_pointer_cast<LivingEntity>(*it);
	player = p;
	followWithCam = p;
	visuals->camera->snapToSanePos(followWithCam);

	ticksAfterPlayedDied = 0;
}

void Game::renderEverything()
{
	visuals->renderStart();
	for (auto& e : World::activeLevel->entities){
		if (e->graphic){
			visuals->renderEntity(e.get());

#ifdef DEBUG_DRAW 
			if (e->collision){
				visuals->renderRect(e->pos.x, e->pos.y, e->pos.w, e->pos.h);
			}
#endif	
		}
	}
#ifdef DEBUG_DRAW 
	visuals->renderRectOverlay(0, 0, visuals->camera->camRect.w, visuals->camera->camRect.w);
	visuals->renderRect(0, 0, World::activeLevel->w, World::activeLevel->h, {0, 255, 255, 255});
#endif	
	
	if (player->behavior && player->behavior->destroyed){
		ticksAfterPlayedDied ++;
		if (ticksAfterPlayedDied >= DELAY_BEFORE_GAMEOVER){
			visuals->renderGameOver();
		}
	} else if (player->golfMode && player->golfMode->active){
		if (player->golfMode->state == AIMING_POWER){
			visuals->renderGolfMeter(AIMING_POWER, player->golfMode->powerCursor, player->golfMode->nPoints);
		} else if (player->golfMode->state == AIMING_HEIGHT){
			visuals->renderGolfMeter(AIMING_HEIGHT, player->golfMode->heightCursor, player->golfMode->nPoints);
		}
	}

	visuals->renderTexts();
	visuals->renderEnd();
}

HandleInputReturnType Game::handleInput()
{
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ){
		if (e.type == SDL_QUIT){
			return HandleInputReturnType::QUIT;
		} else if (e.type == SDL_KEYDOWN){
			
			if (player->behavior && player->behavior->destroyed){
				if (e.key.keysym.scancode == SDL_SCANCODE_SPACE){
					visuals->texts[gameOverTextIndex].display = false;
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
						//Debug
						// std::cout << "Colliders (" <<  player->collision->currentColliders.size() << "):  ";
						// for(auto c : player->collision->currentColliders){
						// 	std:: cout << c->name << " - ";
						// }
						// std::cout << std::endl;

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
			player->behavior->xAcc = player->behavior->walkAcc;
			player->golfMode->setDirection(direction::RIGHT);
		} else if (player->behavior->xSpeed > 0){
			player->behavior->xAcc = 0;
		}

		if (keysPressed[SDL_SCANCODE_LEFT]){
			player->behavior->xAcc = -player->behavior->walkAcc;
			player->golfMode->setDirection(direction::LEFT);
		} else if (player->behavior->xSpeed < 0){
			player->behavior->xAcc = 0;
		}

		if (keysPressed[SDL_SCANCODE_Z]){
			player->behavior->jump();
		}
	}

#ifdef DEBUG_CAMERA
		int camSpeed = 5;
		if (keysPressed[SDL_SCANCODE_D]){
			visuals->camera->camRect.x += camSpeed;
		} 
		if (keysPressed[SDL_SCANCODE_A]){
			visuals->camera->camRect.x -= camSpeed;
		} 
		if (keysPressed[SDL_SCANCODE_W]){
			visuals->camera->camRect.y -= camSpeed;
		} 
		if (keysPressed[SDL_SCANCODE_S]){
			visuals->camera->camRect.y += camSpeed;
		} 
#endif

	return CONTINUE;
}

bool Game::tick()
{
	auto handleInputReturn = handleInput();
	if (handleInputReturn == HandleInputReturnType::RESET){
		setupWorld(World::activeLevel->initialState);
	} else if (handleInputReturn == HandleInputReturnType::QUIT){
		return false;
	}

	float avgFps = visuals->ctx.iteration / (fpsTimer.getTicks() / 1000.f);
	//std::cout << "DEBUG fps" << avgFps << std::endl;
	if (avgFps > 2000000){
		avgFps = 0;
	}

	//Move etc all entities, collision etc
	for (auto& e : World::activeLevel->entities){
		if (e->behavior){
			e->behavior->behave();
		}
	}
	
	//std::cout << "DEBUG: player pos: " << player->pos.x << "," << player->pos.y << std::endl;
	//std::cout << "DEBUG: player y speed: " << player->behavior->ySpeed << std::endl;
	//std::cout << "DEBUG: player grounded: " << player->behavior->grounded << std::endl;

	visuals->camera->followWithCam(followWithCam);

	renderEverything();

#ifdef DEBUG_DRAW 
	//std::cout << "DEBUG: " << avgFps << "," << fpsTextIndex << std::endl;
	visuals->updateText(std::to_string(static_cast<int>(avgFps)), fpsTextIndex);
#endif

 	visuals->ctx.iteration++;
	return true;
}

#ifdef __EMSCRIPTEN__ 
void emscriptenLoop(void *arg)
{	
	Game* game = static_cast<Game*>(arg);
	game->tick();
}
#endif

void Game::start()
{	
	fpsTimer.start();
#ifdef __EMSCRIPTEN__
	const int simulate_infinite_loop = 1;
	const int fps = -1; //defaults to 60, but setting it to 60 seems to trigger EM_TIMING_SETTIMEOUT, causing frames to drop from 60 when theres actual load?
	emscripten_set_main_loop_arg(emscriptenLoop, this, fps, simulate_infinite_loop);
#else
	bool keepGoing = true;
	while(keepGoing){
		capTimer.start();
		
		keepGoing = tick();

		int frameTicks = capTimer.getTicks();
		if( frameTicks < NATIVE_SCREEN_TICK_PER_FRAME ){
			SDL_Delay( NATIVE_SCREEN_TICK_PER_FRAME - frameTicks );
		}
	}
#endif
}
#include <iostream>
#include <memory>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include "living_entity.hpp"
#include "rect.hpp"
#include "animated_graphic.hpp"
#include "entity_factory.hpp"
#include <assert.h>
#include <vector>

LazyFooTimer fpsTimer;
LazyFooTimer capTimer;
Visuals v;
std::shared_ptr<LivingEntity> player;
std::vector<std::shared_ptr<Entity>> entities;

std::shared_ptr<Entity> followWithCam = NULL;

rect playerStartPos = {200, 0};

//#define DEBUG_DRAW
//#define DEBUG_CAMERA
#define DEBUG_CONTROLS

void generateEntities(std::unordered_map<std::string, SDL_Texture*> spritesheets)
{
	EntityFactory factory(spritesheets);

	entities.emplace_back(factory.createGolfClub(50, 0));

	entities.emplace_back( factory.createPlayer(200, 0));
	player = std::static_pointer_cast<LivingEntity>(entities.back());
	followWithCam = player;

	// int blockW = 100;
	// int blockH = 100;
	// for (int i = 0; i < 50; i++){
	// 	Entity* b = new Entity("block" + std::to_string(i), STATIC_SOLID, i * blockW, 400, blockW, blockH);
	// 	b->graphic = std::unique_ptr<Graphic>(new Graphic(sheet2, {0, 0, 32, 32}, b));
	// 	b->collision = std::unique_ptr<Collision>(new Collision(b, true));
	// 	entities.emplace_back(std::shared_ptr<Entity>(b));
	// 	for (int j = 0; j < i ; j++){
	// 		Entity* bv = new Entity("block" + std::to_string(i), STATIC_SOLID, i * blockW, 300 - (blockH/5) * j, blockW, blockH/5);
	// 		bv->graphic = std::unique_ptr<Graphic>(new Graphic(sheet2, {0, 0, 32, 32}, bv));
	// 		bv->collision = std::unique_ptr<Collision>(new Collision(bv, true));
	// 		entities.emplace_back(std::shared_ptr<Entity>(bv));
	// 	}
	// }

	// followWithCam = player;
}

int main (int argc, char* argv[])
{
	assert(v.spritesheets.size() > 0);
	generateEntities(v.spritesheets);


	int countedFrames = 0;
	const int FPS = 60;
	const int SCREEN_TICK_PER_FRAME = 1000 / FPS;

	bool keepGoing = true;

	fpsTimer.start();
	
	bool keysPressed[322] = {false};

	while(keepGoing){
		capTimer.start();

#ifndef DEBUG_CAMERA
		v.camera->camRect.x = followWithCam->pos.x - v.camera->camRect.w / 2;
		v.camera->camRect.y = followWithCam->pos.y - v.camera->camRect.h / 2;
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
		auto playerAnimatedGraphic = (AnimatedGraphic*)(player->graphic.get());
		#define SLOW_DOWN_AMOUNT 0.2
		if (keysPressed[SDL_SCANCODE_RIGHT]){
			player->behavior->addXSpeed(player->behavior->walkAcc);
			if (player->behavior->grounded){
				playerAnimatedGraphic->changeState(AnimationState::WALK);
			}
		} else if (player->behavior->xSpeed > 0){
			player->behavior->addXSpeed(-SLOW_DOWN_AMOUNT, true);
			playerAnimatedGraphic->changeState(AnimationState::DEFAULT);
		}
		if (keysPressed[SDL_SCANCODE_LEFT]){
			player->behavior->addXSpeed(-player->behavior->walkAcc);
			if (player->behavior->grounded){
				playerAnimatedGraphic->changeState(AnimationState::WALK);
			}
		} else if (player->behavior->xSpeed < 0){
			player->behavior->addXSpeed(SLOW_DOWN_AMOUNT, true);
			playerAnimatedGraphic->changeState(AnimationState::DEFAULT);
		}
		if (keysPressed[SDL_SCANCODE_SPACE]){
			player->behavior->jump();
			playerAnimatedGraphic->changeState(AnimationState::JUMP);
		}

		//If movement was stopped by wall, dont keep walk animation, same when landing
		if (playerAnimatedGraphic->curAnimationState == AnimationState::WALK && player->behavior->xSpeed == 0){
			playerAnimatedGraphic->changeState(AnimationState::DEFAULT);
		}

		if (playerAnimatedGraphic->curAnimationState == AnimationState::JUMP && player->behavior->grounded){
			playerAnimatedGraphic->changeState(AnimationState::DEFAULT);
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

		//Move etc all entities, collision etc
		for (auto& e : entities){
			if (e->behavior){
				e->behavior->behave(entities);
			}
		}
		
		// Render everything
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


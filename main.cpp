#include <iostream>
#include <memory>
#include "visuals.hpp"
#include "sdl_utils.hpp"
#include "entity.hpp"
#include "living_entity.hpp"
#include "rect.hpp"
#include "animated_graphic.hpp"

#include <vector>

LazyFooTimer fpsTimer;
LazyFooTimer capTimer;
Visuals v;
Entity* player = NULL;
std::vector<std::unique_ptr<Entity>> entities;
rect playerStartPos = {200, 0};

#define DEBUG_DRAW
//#define DEBUG_CAMERA
#define DEBUG_CONTROLS

void generateEntities()
{
	// Get all spritesheets
	auto sheet1 = v.getSpritesheet("spritesheet1");
	auto sheet2 = v.getSpritesheet("spritesheet2");
	auto sheet3 = v.getSpritesheet("spritesheet3");

	// Setup entities
	Entity* e = new LivingEntity("player", playerStartPos.x, playerStartPos.y, 70, 100);
	e->graphic = std::unique_ptr<Graphic>(new AnimatedGraphic(e));

	auto animatedGraphic = (AnimatedGraphic*)e->graphic.get();

	auto walkAnimation = new Animation(sheet1);
	walkAnimation->frames.push_back(std::make_unique<Frame>(32, 0, 32, 32));
	walkAnimation->frames.push_back(std::make_unique<Frame>(64, 0, 32, 32));
	animatedGraphic->animations.insert({AnimationState::WALK, std::unique_ptr<Animation>(walkAnimation)});

	auto idleAnimation = new Animation(sheet1);
	idleAnimation->frames.push_back(std::make_unique<Frame>(0, 0, 32, 32));
	idleAnimation->no = true;
	animatedGraphic->animations.insert({AnimationState::DEFAULT, std::unique_ptr<Animation>(idleAnimation)});

	auto jumpAnimation = new Animation(sheet1);
	jumpAnimation->frames.push_back(std::make_unique<Frame>(0, 0, 32, 32));
	jumpAnimation->no = true;
	animatedGraphic->animations.insert({AnimationState::JUMP, std::unique_ptr<Animation>(jumpAnimation)});

	e->behavior = std::unique_ptr<Behavior>(new Behavior(e));
	//e->behavior->gravity = false;
	e->collision = std::unique_ptr<Collision>(new Collision(e, true));
	player = e;
	entities.emplace_back(std::unique_ptr<Entity>(e));

	int blockW = 100;
	int blockH = 100;
	for (int i = 0; i < 10; i++){
		Entity* b = new Entity("block" + std::to_string(i), i * blockW, 400, blockW, blockH);
		b->graphic = std::unique_ptr<Graphic>(new Graphic(sheet2, {0, 0, 32, 32}, b));
		b->collision = std::unique_ptr<Collision>(new Collision(b, true));
		entities.emplace_back(std::unique_ptr<Entity>(b));
		for (int j = 0; j < i ; j++){
			Entity* bv = new Entity("block" + std::to_string(i), i * blockW, 300 - (blockH/5) * j, blockW, blockH/5);
			bv->graphic = std::unique_ptr<Graphic>(new Graphic(sheet2, {0, 0, 32, 32}, bv));
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
		v.camera->camRect.x = player->pos.x - v.camera->camRect.w / 2;
		v.camera->camRect.y = player->pos.y - v.camera->camRect.h / 2;
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
			if (player->behavior->grounded){
				((AnimatedGraphic*)(player->graphic.get()))->changeState(AnimationState::WALK);
			}
		} else if (player->behavior->xSpeed > 0){
			player->behavior->addXSpeed(-SLOW_DOWN_AMOUNT, true);
			((AnimatedGraphic*)(player->graphic.get()))->changeState(AnimationState::DEFAULT);
		}
		if (keysPressed[SDL_SCANCODE_LEFT]){
			player->behavior->addXSpeed(-player->behavior->walkAcc);
			if (player->behavior->grounded){
				((AnimatedGraphic*)(player->graphic.get()))->changeState(AnimationState::WALK);
			}
		} else if (player->behavior->xSpeed < 0){
			player->behavior->addXSpeed(SLOW_DOWN_AMOUNT, true);
			((AnimatedGraphic*)(player->graphic.get()))->changeState(AnimationState::DEFAULT);
		}

		if (keysPressed[SDL_SCANCODE_UP]){
			player->behavior->jump();
			((AnimatedGraphic*)(player->graphic.get()))->changeState(AnimationState::JUMP);
		}

#ifdef DEBUG_CONTROLS
		if (keysPressed[SDL_SCANCODE_SPACE]){
			player->pos.x = playerStartPos.x;
			player->pos.y = playerStartPos.y;
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
			e->prevPos = e->pos;
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
					//std::cout << "DEBUG: collision: " << e->name << " and " << collidee->name << std::endl;
					collidee->collision->effect(e.get(), intersect);
				}

				// Check if ground underneath so flip grounded to false 
				// TODO: would be nice if isolated 'pushout' collision from Collision::effect and also used it here
				if (e->behavior && collidee->collision->solid && e->collision->solid){
					int originalY = e->pos.y;

					e->pos.y += 1;
					auto intersect = Collision::checkCollision(e.get(), collidee.get());
					bool collision = intersect.w > 0 && intersect.h > 0;
					if (collision){
						//std::cout << "DEBUG: has ground underneath: " << e->name << " and " << collidee->name << std::endl;
						hasGroundUnderneath = true;
						groundIntersect = intersect;
					}

					e->pos.y = originalY;
				}
			}

			if (!hasGroundUnderneath && e->behavior && e->behavior->gravity){
				e->behavior->grounded = false;
			} 
		}
		
		// Render everything
		v.renderStart();
		for (auto& e : entities){
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


#include "entity_factory.hpp"
#include <memory>
//#include "graphic.hpp"
#include "animated_graphic.hpp"
#include <assert.h>

EntityFactory::EntityFactory(std::unordered_map<std::string, SDL_Texture*> spritesheets)
:spritesheets(spritesheets)
{}

LivingEntity* EntityFactory::createPlayer(int x, int y)
{
	auto sheet = spritesheets.at("spritesheet1");

	auto e = new LivingEntity("player", x, y, 70, 100);
	e->graphic = std::unique_ptr<Graphic>(new AnimatedGraphic(e));

	auto animatedGraphic = (AnimatedGraphic*)e->graphic.get();

	auto walkAnimation = new Animation(sheet);
	walkAnimation->frames.push_back(std::make_unique<Frame>(32, 0, 32, 32));
	walkAnimation->frames.push_back(std::make_unique<Frame>(64, 0, 32, 32));
	animatedGraphic->animations.insert({AnimationState::WALK, std::unique_ptr<Animation>(walkAnimation)});

	auto idleAnimation = new Animation(sheet);
	idleAnimation->frames.push_back(std::make_unique<Frame>(0, 0, 32, 32));
	idleAnimation->no = true;
	animatedGraphic->animations.insert({AnimationState::DEFAULT, std::unique_ptr<Animation>(idleAnimation)});

	auto jumpAnimation = new Animation(sheet);
	jumpAnimation->frames.push_back(std::make_unique<Frame>(32, 0, 32, 32));
	jumpAnimation->no = true;
	animatedGraphic->animations.insert({AnimationState::JUMP, std::unique_ptr<Animation>(jumpAnimation)});

	e->behavior = std::unique_ptr<Behavior>(new Behavior(e));
	//e->behavior->gravity = false;
	e->collision = std::unique_ptr<Collision>(new Collision(e, true));
	return e;
}
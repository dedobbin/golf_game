#include "entity_factory.hpp"
#include <memory>
//#include "graphic.hpp"
#include "animated_graphic.hpp"
#include "enemy_behavior.hpp"

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

	e->behavior = std::unique_ptr<Behavior>(new Behavior(e, true));
	//e->behavior->gravity = false;
	e->collision = std::unique_ptr<Collision>(new Collision(e, true));
	return e;
}

Item* EntityFactory::createGolfClub(int x, int y)
{
	auto sheet = spritesheets.at("spritesheet2");

	auto club = new Item("golf_club", x, y, 32, 100);
	auto clubGraphic = new Graphic(sheet, {0, 32, 6 , 25},  club);
	club->graphic = std::unique_ptr<Graphic>(clubGraphic);
	club->behavior = std::make_unique<Behavior>(club);
	club->collision = std::make_unique<Collision>(club);
	return club;
}

Entity* EntityFactory::createBall(int x, int y)
{
	auto sheet = spritesheets.at("spritesheet2");

	auto ball = new Entity("golf_ball", BALL, x, y, 20, 20);
	auto graphic = new Graphic(sheet, {32, 32, 6 , 6},  ball);
	ball->graphic = std::unique_ptr<Graphic>(graphic);
	ball->behavior = std::make_unique<Behavior>(ball);
	ball->collision = std::make_unique<Collision>(ball);
	return ball;
}

Entity* EntityFactory::createBlock(int x, int y, int w, int h)
{
	auto sheet = spritesheets.at("spritesheet2");

	Entity* b = new Entity("block", STATIC_SOLID, x,  y, w, h);
	b->graphic = std::unique_ptr<Graphic>(new Graphic(sheet, {0, 0, 32, 32}, b));
	b->collision = std::unique_ptr<Collision>(new Collision(b, true));
	return b;
}

LivingEntity* EntityFactory::createEnemy(int x, int y)
{
	auto sheet = spritesheets.at("spritesheet4");

	LivingEntity* e = new LivingEntity("enemy", x, y, 70, 100);
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

	e->behavior = std::unique_ptr<EnemyBehavior>(new EnemyBehavior(e));
	//e->behavior->gravity = false;
	e->collision = std::unique_ptr<Collision>(new Collision(e, true));
	return e;
}
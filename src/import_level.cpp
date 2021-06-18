#include "import_level.hpp"
#include "utils/json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <regex>
#include <dirent.h>


#include "utils/utils.hpp"
#include "entities/living_entity.hpp"
#include "entities/item.hpp"
#include "entity_properties/item_behavior.hpp"
#include "entity_properties/enemy_behavior.hpp"

std::unordered_map<std::string, SDL_Texture*> spriteSheets;

void parseGraphic(nlohmann::json jGraphic, Entity* owner)
{
	if (jGraphic.is_null()){
		return;
	}
	/* If graphic has own spritesheet + frame, 'non-animation' constructor for graphic */
	if (!jGraphic["spritesheet"].is_null() && !jGraphic["frame"].is_null()){
		auto sheet = spriteSheets[jGraphic["spritesheet"]];
		SDL_Rect srcPos = {jGraphic["frame"][0], jGraphic["frame"][1], jGraphic["frame"][2], jGraphic["frame"][3]};
		
		owner->graphic = std::make_unique<Graphic>(sheet, srcPos, owner);
		return;
	}
	
	/* Graphic does not have own spritesheet + frame, so it's animated, get frames */
	auto graphic = new Graphic(owner);
	for(auto jAnimation : jGraphic["animations"]){
		std::string sheetName = jAnimation["spritesheet"];
		if (spriteSheets.find(sheetName) == spriteSheets.end()){
			throw std::runtime_error("import_level: Can't find spritesheet, " + sheetName);
		}

		auto animation = new Animation(spriteSheets[sheetName]);

		for (auto jFrame : jAnimation["frames"]){
			int x = jFrame[0];
			int y = jFrame[1];
			int w = jFrame[2];
			int h = jFrame[3];
			animation->frames.push_back(std::make_unique<Frame>(x,y,w,h));
		}

		if (!jAnimation["loop"].is_null()){
			animation->loop = jAnimation["loop"];
		}

		if (!jAnimation["speed"].is_null()){
			animation->animationSpeed = jAnimation["speed"];
		}

		if (jAnimation["type"] == "default"){
			graphic->animations[DEFAULT] = std::unique_ptr<Animation>(animation);
		} else if (jAnimation["type"] == "walk"){
			graphic->animations[WALK] = std::unique_ptr<Animation>(animation);
		} else if (jAnimation["type"] == "jump"){
			graphic->animations[JUMP] = std::unique_ptr<Animation>(animation);
		} else if (jAnimation["type"] == "fall"){
			graphic->animations[FALL] = std::unique_ptr<Animation>(animation);
		} else if (jAnimation["type"] == "dead"){
			graphic->animations[DEAD] = std::unique_ptr<Animation>(animation);
		}
	}

	owner->graphic = std::unique_ptr<Graphic>(graphic);
}

void parseBehavior(nlohmann::json jBehavior, Entity* owner)
{
	if (jBehavior.is_null()){
		return;
	}

	Behavior* behavior = NULL;
	if (owner->type == entityType::ITEM){
		behavior = new ItemBehavior((Item*)owner);
	} else if (jBehavior["type"] == "enemy_a"){//TODO: check for other enemy types..
		behavior = new EnemyBehavior((LivingEntity*) owner);
	} else {
		bool pickupItems = jBehavior["pickup_items"].is_boolean() && jBehavior["pickup_items"];
		behavior = new Behavior(owner, pickupItems);
	}

	if (jBehavior["type"] == "player"){
		behavior->type = BehaviorType::PLAYER;
	} else if (jBehavior["type"] == "enemy_a"){
		behavior->type = BehaviorType::ENEMY_A;
	}

	if (!jBehavior["walk_acc"].is_null()){
		behavior->walkAcc = jBehavior["walk_acc"];
	} else if (!jBehavior["max_x_speed"].is_null()){
		behavior->maxXSpeed = jBehavior["max_x_speed"];
	} else if (!jBehavior["max_y_speed"].is_null()){
		behavior->maxYSpeed = jBehavior["max_y_speed"];
	} else if (!jBehavior["max_walk_speed"].is_null()){
		behavior->maxWalkSpeed = jBehavior["max_walk_speed"];
	} else if (!jBehavior["gravity"].is_null()){
		behavior->gravity = jBehavior["gravity"];
	} else if (!jBehavior["friction_ground"].is_null()){
		behavior->frictionGround = jBehavior["friction_ground"];
	}

	owner->behavior = std::unique_ptr<Behavior>(behavior);
}

void parseCollision(nlohmann::json jCollision, Entity* owner)
{
	if (jCollision.is_null()){
		return;
	}
	auto collision = new Collision(owner);
	collision->solid = !jCollision["solid"].is_null() && jCollision["solid"];
	owner->collision = std::unique_ptr<Collision>(collision);
}

void parseGolfMode(nlohmann::json jCollision, LivingEntity* owner)
{
	owner->golfMode = std::make_unique<GolfMode>(owner);
}

Entity* parseEntity(nlohmann::json jEntity)
{
	Entity* entity= NULL;

	if (jEntity["type"] == "living"){
		entity = new LivingEntity (
			jEntity["name"],
			jEntity["pos"][0],
			jEntity["pos"][1],
			jEntity["pos"][2],
			jEntity["pos"][3]
		);
	} else if (jEntity["type"] == "item"){
		entity = new Item(
			jEntity["name"],
			jEntity["pos"][0],
			jEntity["pos"][1],
			ItemType::CLUB, //placeholder
			jEntity["pos"][2],
			jEntity["pos"][3]
		);

		if (jEntity["item_type"] == "club"){
			((Item*)entity)->itemType = ItemType::CLUB;
		}
	} else {
		entity = new Entity(
			jEntity["name"],
			entityType::STATIC_SOLID, //placeholder
			jEntity["pos"][0],
			jEntity["pos"][1],
			jEntity["pos"][2],
			jEntity["pos"][3]
		);
		
		/* Set entity types of objects that not have it set by child like LivingEntity and Item */
		if (jEntity["type"] == "static_solid"){
			entity->type = entityType::STATIC_SOLID;
		} else if (jEntity["type"]  == "ball"){
			entity->type = entityType::BALL;
		} else if (jEntity["type"]  == "spikes"){
			entity->type = entityType::SPIKES;
		}
	}

	parseGraphic(jEntity["graphic"], entity);
	parseBehavior(jEntity["behavior"], entity);
	parseCollision(jEntity["collision"], entity);

	if (entity->type == entityType::LIVING){
		parseGolfMode(jEntity["golf_mode"], (LivingEntity*)entity);
	}

	return entity;
}

void fillWorld(nlohmann::json j, std::shared_ptr<LevelData> level)
{
	level->w = j["world_w"];
	level->h = j["world_h"];
	level->gravity = j["world_gravity"];

	for (auto jEntity : j["entities"]){
		auto entity = parseEntity(jEntity);
		std::cout << "DEBUG: Imported " << entity->name << std::endl;
		level->entities.emplace_back(entity);
	}

	return;
}

std::shared_ptr<LevelData> Import::importLevel(nlohmann::json data, std::unordered_map<std::string, SDL_Texture*> _spriteSheets)
{	
	spriteSheets = _spriteSheets;
	auto levelData = std::make_shared<LevelData>();
	//levelData->filename = filename;

	assert(spriteSheets.size() > 0);

	// std::string path = RESOURCES_PATH + std::string("/levels/") + filename;
 	// std::ifstream ifs(path);
  	// // std::string content((std::istreambuf_iterator<char>(ifs)),
    // //                    (std::istreambuf_iterator<char>()));

	// nlohmann::json j;
	// ifs >> j;

	fillWorld(data, levelData);
	return levelData;
}
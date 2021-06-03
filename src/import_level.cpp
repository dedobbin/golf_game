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

	//TODO: parse entity attributes

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

std::shared_ptr<LevelData> Import::importLevel(std::string filename, std::unordered_map<std::string, SDL_Texture*> _spriteSheets)
{	
	// std::cout << "Testing JSON" << std::endl;
	// nlohmann::json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
	// std::cout << j["happy"];
	// exit(1);


	spriteSheets = _spriteSheets;
	auto levelData = std::make_shared<LevelData>();
	levelData->filename = filename;

	assert(spriteSheets.size() > 0);

	std::string path = RESOURCES_PATH + std::string("/levels/") + filename;
 	std::ifstream ifs(path);
  	// std::string content((std::istreambuf_iterator<char>(ifs)),
    //                    (std::istreambuf_iterator<char>()));

	std::cout << "DEBUG, loading " << path << std::endl;
	nlohmann::json j;
	ifs >> j;
	fillWorld(j, levelData);
}
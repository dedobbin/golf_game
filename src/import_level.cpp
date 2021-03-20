#include "import_level.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <regex>
#include <dirent.h>

#include "world.hpp"
#include "utils/utils.hpp"
#include "entities/living_entity.hpp"
#include "entities/item.hpp"

std::unordered_map<std::string, SDL_Texture*> spriteSheets;

struct Block
{
	std::string rawContent = "";
	std::unordered_map<std::string, std::string> attributes;
	std::vector<std::shared_ptr<Block>> blocks;
};

std::shared_ptr<Block> blockify(std::string raw)
{
	std::string curString = "";
	std::string curAttribute = "";
	std::vector<std::shared_ptr<Block>> blocks;
	
	auto topBlock = std::make_shared<Block>();
	topBlock->attributes["type"] = "top";

	raw.erase(std::remove_if(raw.begin(), raw.end(), 
		[](unsigned char x){
			//return std::isspace(x);
			return x == '\n' || x == '\t';
		}), raw.end()
	);

	blocks.push_back(topBlock); 
	while(raw != "")
	{
		curString += raw[0];

		if (raw[0] == '{'){
			blocks.push_back(std::make_shared<Block>());
			curString = "";
		} else if (raw[0] == '}'){
			auto b = blocks.back();
			blocks.pop_back();
			blocks.back()->blocks.push_back(b);
			curString = "";
		} else {
			blocks.back()->rawContent += raw[0];

			if (raw[0] == '='){
				curAttribute = curString.substr(0, curString.size() - 1);//dont take =
				curString = "";
				//blocks.back()->attributes[curAttribute] = "";
			}else if (raw[0] == ';'){
				blocks.back()->attributes[curAttribute] = curString.substr(0, curString.size() - 1); //dont take ;
				curAttribute = "";
				curString = "";
			}
		}

		raw = raw.substr(1, raw.size() - 1);
	}

	return blocks.back();
}

void printBlock(std::shared_ptr<Block> block)
{
	//std::cout << block->rawContent << std::endl;
	std::cout << "block type: " << block->attributes["type"] << std::endl;
	for(auto iter = block->attributes.begin(); iter != block->attributes.end(); ++iter){
		std::string k =  iter->first;
		if (k == "type") continue;
		std::string v =  iter->second;
		std::cout << "\t" << k << ":" << v << std::endl;
	}

	for(auto &b : block->blocks){
		printBlock(b);
	}
}

std::vector<rect> parseFramePosStr(std::string frameStr)
{
	//frameStr = "[32,0,32,32],[64,0,32,32],[12,34,56,78]"; 

	std::vector<rect> output; 

	frameStr.erase(std::remove_if(frameStr.begin(), frameStr.end(), 
		[](unsigned char x){
			return std::isspace(x);
		}), frameStr.end()
	);

	const std::regex r("(\\[[0-9]+,[0-9]+,[0-9]+,[0-9]+\\],*)");  
	
	while(frameStr != ""){
		std::smatch sm;
		if (regex_search(frameStr, sm, r)) {
			if (sm.size() > 0){
				//std::cout <<  sm.str(1) << std::endl;
				std::string str = sm.str(1);
				
				int rStrip = 1;
				if (str[str.size()-1] == ','){
					//std::cout << "sdf" << std::endl;
					rStrip = 2;
				}

				auto posData = explode(  str.substr(1, str.size()-rStrip-1) ,',');
				rect r = {std::stoi(posData[0]),std::stoi(posData[1]),std::stoi(posData[2]),std::stoi(posData[3])};
				output.push_back(r);


				frameStr = frameStr.substr(sm.str(1).size(), frameStr.size() - sm.str(1).size());
			}
		} else {
			std::cout << "DEBUG: invalid graphic pos data, remaining unparsed data: " << frameStr << std::endl;
			break;
		}
	}
	return output;
}

void parseMetaData(std::shared_ptr<Block> block)
{
	auto attr = block->attributes;
	World::name = attr["name"];
	World::w = std::stoi(attr["world_w"]);
	World::h = std::stoi(attr["world_h"]);
	World::gravity = std::stof(attr["world_gravity"]);
}

Graphic* parseGraphic(std::shared_ptr<Block> graphicBlock, Entity* owner)
{
	auto graphic = new Graphic(owner);
	for (auto& graphicProperties: graphicBlock->blocks){
		auto graphAttr = graphicProperties->attributes;
		if (graphAttr["type"] == "animation"){
			auto animation = std::make_unique<Animation>(spriteSheets.at(graphAttr["spritesheet"]));
			for (auto& rect : parseFramePosStr(graphAttr["frames"])){
				animation->frames.push_back(std::make_unique<Frame>(rect.x, rect.y, rect.w, rect.h));
			}

			if (graphAttr.find("speed") != graphAttr.end()){
				animation->animationSpeed = std::stoi(graphAttr["speed"]);
			} 
			if (graphAttr.find("loop") != graphAttr.end()){
				animation->loop = (bool)std::stoi(graphAttr["loop"]);
			} 

			if (graphAttr["animation_type"]=="default"){
				graphic->animations[DEFAULT] = std::move(animation);
			} else if (graphAttr["animation_type"]=="walk") {
				graphic->animations[WALK] =  std::move(animation);
			} else if (graphAttr["animation_type"]=="jump") {
				graphic->animations[JUMP] =  std::move(animation);
			} else if (graphAttr["animation_type"]=="fall") {
				graphic->animations[FALL] =  std::move(animation);
			} else if (graphAttr["animation_type"]=="dead") {
				graphic->animations[DEAD] =  std::move(animation);
			}
		}
	} 
	return graphic;
}

Behavior* parseBehavior(std::shared_ptr<Block> block, Entity* owner)
{
	bool pickupItems = false;
	if (block->attributes.find("pickup_items") != block->attributes.end()){
		pickupItems = (bool)std::stoi(block->attributes["pickup_items"]);
	}
	auto behavior = new Behavior(owner, pickupItems);

	if (block->attributes.find("walk_acc") != block->attributes.end()){
		behavior->walkAcc = std::stof(block->attributes["walk_acc"]);
	} else if (block->attributes.find("max_x_speed") != block->attributes.end()){
		behavior->maxXSpeed = std::stof(block->attributes["max_x_speed"]);
	} else if (block->attributes.find("max_walk_speed") != block->attributes.end()){
		behavior->maxWalkSpeed = std::stof(block->attributes["max_walk_speed"]);
	} else if (block->attributes.find("max_y_speed") != block->attributes.end()){
		behavior->maxYSpeed = std::stof(block->attributes["max_y_speed"]);
	} else if (block->attributes.find("gravity") != block->attributes.end()){
		behavior->gravity = (bool)std::stoi(block->attributes["gravity"]);
	} else if (block->attributes.find("friction_ground") != block->attributes.end()){
		behavior->frictionGround = std::stof(block->attributes["friction_ground"]);
	}

	return behavior;
}

Collision* parseCollision(std::shared_ptr<Block> block, Entity* owner)
{
	auto collision = new Collision(owner);

 	if (block->attributes.find("solid") != block->attributes.end()){
		collision->solid = (bool)std::stoi(block->attributes["solid"]);
	}

	return collision;
}

GolfMode* parseGolfMode(std::shared_ptr<Block> block, LivingEntity* owner)
{
	auto golfMode = new GolfMode(owner);
	return golfMode;
}

Entity* parseEntity(std::shared_ptr<Block> block)
{
	Entity* entity = NULL;
	auto pos = explode(block->attributes["pos"], ',');
	
	if (block->attributes["entity_type"] == "living"){
		entity = new LivingEntity(
			block->attributes["name"],
			LivingEntityType::ENEMY_A, //placeholder
			std::stoi(pos[0]),
			std::stoi(pos[1]),
			std::stoi(pos[2]),
			std::stoi(pos[3])
		);
		if (block->attributes["living_entity_type"] == "player"){
			((LivingEntity*)entity)->livingEntityType = LivingEntityType::PLAYER;
		} else if (block->attributes["living_entity_type"] == "enemy_a"){
			((LivingEntity*)entity)->livingEntityType = LivingEntityType::ENEMY_A;
		}
	} else if (block->attributes["entity_type"] == "item"){
		entity = new Item(
			block->attributes["name"],
			std::stoi(pos[0]),
			std::stoi(pos[1]),
			ItemType::CLUB, //placeholder
			std::stoi(pos[2]),
			std::stoi(pos[3])
		);

		if (block->attributes["item_type"] == "club"){
			((Item*)entity)->itemType = ItemType::CLUB;
		}
	} else {
		entity = new Entity(
			block->attributes["name"],
			entityType::STATIC_SOLID, //placeholder
			std::stoi(pos[0]),
			std::stoi(pos[1]),
			std::stoi(pos[2]),
			std::stoi(pos[3])
		);
		
		/* Set entity types of objects that not have it set by child like LivingEntity and Item */
		if (block->attributes["living_entity_type"] == "static_solid"){
			entity->type = entityType::STATIC_SOLID;
		} else if (block->attributes["living_entity_type"] == "ball"){
			entity->type = entityType::BALL;
		}
	}

	/* Entitiy properties like gravity, behavior etc */
	for (auto& property : block->blocks){
		auto propAttr = property->attributes;
		if (propAttr["type"] == "graphic"){ 
			entity->graphic = std::unique_ptr<Graphic>(parseGraphic(property, entity));
			std::cout << "Level import: Created graphic for " << entity->name << std::endl;
		} else if (propAttr["type"] == "behavior"){
			entity->behavior = std::unique_ptr<Behavior>(parseBehavior(property, entity));
		} else if (propAttr["type"] == "collision"){
			entity->collision = std::unique_ptr<Collision>(parseCollision(property, entity));
		} else if (propAttr["type"] == "golf_mode"){
			if (entity->type != entityType::LIVING){
				std::cout << "Level import: Trying to set golf mode on non-living entity, nope.." <<std::endl;
			} else {
				((LivingEntity*)entity)->golfMode = std::unique_ptr<GolfMode>(parseGolfMode(property, (LivingEntity*)entity));
			}
		}
	}

	return entity;
}

void fillWorld(std::shared_ptr<Block> block)
{
	if (block->attributes["type"] == "metadata"){
		parseMetaData(block);
		std::cout << "Level import: metadata set, world size: " << World::w << "x" << World::h << ", gravity: " << World::gravity << std::endl; 
	} else if (block->attributes["type"]=="entity"){
		auto entity = parseEntity(block);
		std::cout << "Level import: Created entity " << entity->name << " (" << entity->type << ")" << std::endl;
		World::entities.emplace_back(entity);
	}

	for(auto &b : block->blocks){
		fillWorld(b);
	}
}

void Import::importLevel(std::string filePath, std::unordered_map<std::string, SDL_Texture*> _spriteSheets)
{	
	spriteSheets = _spriteSheets;
	assert(spriteSheets.size() > 0);

 	std::ifstream ifs(filePath);
  	std::string content((std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));

	// std::string content = "{type=metadata;}{type=entity;{type=graphic;sprite=3;}}";
	
	auto topBlock = blockify(content);
	//printBlock(topBlock);
	//printBlock(topBlock->blocks[1]);
	fillWorld(topBlock);
}

//for testing
// int main(int argc, char* argv[])
// {
// 	auto visuals = std::make_shared<Visuals>();
// 	visuals->loadSpritesheets(visuals->defaultSpritesheetPath);
// 	Import::importLevel(RESOURCES_PATH + std::string("/levels/1.wsp"), visuals->spritesheets);
// 	return 0;
// }
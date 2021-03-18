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

void fillWorld(std::shared_ptr<Block> block)
{
	auto attr = block->attributes;
	if (attr["type"] == "metadata"){
		World::name = attr["name"];
		World::w = std::stoi(attr["world_w"]);
		World::h = std::stoi(attr["world_h"]);
		World::gravity = std::stof(attr["world_gravity"]);
	} else if (attr["type"]=="entity"){
		Entity* entity = NULL;
		auto pos = explode(attr["pos"], ',');
		
		if (attr["entity_type"] == "living"){
			entity = new LivingEntity(
				attr["name"],
				LivingEntityType::ENEMY_A, //placeholder
				std::stoi(pos[0]),
				std::stoi(pos[1]),
				std::stoi(pos[2]),
				std::stoi(pos[3])
			);
			//TODO: set living entity type

		} else {
			//TODO: set entity type
			entity = new Entity(
				attr["name"],
				entityType::STATIC_SOLID, //placeholder
				std::stoi(pos[0]),
				std::stoi(pos[1]),
				std::stoi(pos[2]),
				std::stoi(pos[3])
			);
		}

		/* Entitiy properties like gravity, behavior etc */
		for (auto& property : block->blocks){
			auto propAttr = property->attributes;
			if (propAttr["type"] == "graphic"){ 
				entity->graphic = std::make_unique<Graphic>(entity);
				for (auto& graphicProperties: property->blocks){
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
						std::cout << "DEBUG: " << graphAttr["animation_type"] << " speed:" << animation->animationSpeed << std::endl;
						if (graphAttr["animation_type"]=="default"){
							entity->graphic->animations[DEFAULT] = std::move(animation);
						} else if (graphAttr["animation_type"]=="walk") {
							entity->graphic->animations[WALK] =  std::move(animation);
						} else if (graphAttr["animation_type"]=="jump") {
							entity->graphic->animations[JUMP] =  std::move(animation);
						} else if (graphAttr["animation_type"]=="fall") {
							entity->graphic->animations[FALL] =  std::move(animation);
						} else if (graphAttr["animation_type"]=="dead") {
							entity->graphic->animations[DEAD] =  std::move(animation);
						}
					}
				}
			}
		}

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
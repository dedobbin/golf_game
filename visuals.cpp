#include <iostream>
#include <dirent.h>
#include "visuals.hpp"
#include "living_entity.hpp"
#include <iostream>
#include "utils.hpp"

int curTextIndex = 0;
int gameOverTextIndex = -1;

Visuals::Visuals()
{
 	SDL_Init(SDL_INIT_VIDEO);
 	
	if (TTF_Init() == -1){
		std::cout << "Failed to init SDL_ttf: " <<  TTF_GetError() << std::endl;
	}

	gFont = TTF_OpenFont("assets/OpenSans-Regular.ttf", 28);
	if (gFont == NULL){
		std::cout << "Failed to load font: " <<  TTF_GetError() << std::endl;
	}


	SDL_Window* window;
	SDL_Renderer* renderer;
 
    SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);

    ctx.renderer = renderer;
    ctx.iteration = 0;

	camera = std::make_unique<Camera>(200, 0, 900, 700);
}

Visuals::~Visuals()
{
	for (auto sheet : spritesheets){
		SDL_DestroyTexture(sheet.second);
	}

	for (auto text : texts){
		SDL_DestroyTexture(text.second.texture);
	}

	TTF_CloseFont(gFont);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Visuals::loadSpritesheets(std::string path)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (strcmp(ent->d_name, ".") == 0 ^ strcmp(ent->d_name, "..") == 0){
				continue;
			}
			
			if (!ends_with(ent->d_name, ".png")){
				continue;
			}

			auto location = path + "/" + ent->d_name;
			strip_ext(ent->d_name);
			spritesheets[ent->d_name] = loadTexture(location);
			//std::cout << "DEBUG: Added spritesheet, " << ent->d_name << ": " << path + "/" + ent->d_name << std::endl;
		}
		closedir (dir);
		return true;
	} else {
		std::cerr << "Can't load spritesheets " << strerror(errno) << std::endl;
		return false;
	}

}

void Visuals::loadSpritesheet(std::string name)
{
	auto path = defaultSpritesheetPath + "/" + name + ".png";
	spritesheets[name] = loadTexture(path);
	//std::cout << "DEBUG: Added spritesheet, " << name << ": " << path << std::endl;
}

SDL_Texture* Visuals::getSpritesheet(std::string name)
{
	if (spritesheets.find(name) == spritesheets.end()){
		loadSpritesheet(name);
	}
	return spritesheets.at(name);
}

SDL_Texture* Visuals::loadTexture(std::string path) const
{
	SDL_Surface *image = IMG_Load(path.c_str());
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0x64, 0x64, 0x64));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ctx.renderer, image);
    SDL_FreeSurface(image);
	return tex;
}


void Visuals::renderStart()
{
	SDL_SetRenderDrawColor( ctx.renderer, 148, 62, 62, 255 );
	//SDL_SetRenderDrawColor( ctx.renderer, 255, 62, 255, 255 );
    SDL_RenderClear( ctx.renderer );
}

void Visuals::renderEnd()
{
	SDL_RenderPresent(ctx.renderer);
}

void Visuals::renderEntity(Entity* entity)
{
	if (!camera->inView(entity->pos.x,entity->pos.y,entity->pos.w,entity->pos.h))
		return;
	if (entity->type == ITEM && ((Item*)entity)->owner){
		/* When item is owned, owning entity takes care of drawing, easier to handle items in inventory (not draw) + z logic.. */
		return;
	}
 	entity->graphic->render(ctx.renderer, camera.get());
}

void Visuals::renderRect(int x, int y, int w, int h)
{
	if (!camera->inView(x, y, w, h))
		return;
	
	SDL_SetRenderDrawColor(ctx.renderer, 255, 0, 255, 255);
	SDL_Rect rect = {x - camera->camRect.x, y - camera->camRect.y, w, h};
	SDL_RenderDrawRect(ctx.renderer, &rect);
}

void Visuals::renderRectOverlay(int x, int y, int w, int h)
{
	SDL_SetRenderDrawColor(ctx.renderer, 255, 0, 255, 255);
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderDrawRect(ctx.renderer, &rect);
}

void Visuals::renderGolfMeter(GolfState state, int level, int nPoints)
{
	// std::cout << "DEBUG render golf meter" << std::endl;
	// SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

	if (state == AIMING_POWER){
		SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);
	} else if (state == AIMING_HEIGHT){
		SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 255, 255);
	}

	int x = 10, y = 10, w = 3, h = 30;
	int pointSpacing = 5;

	SDL_Rect cursor;

	SDL_Rect rects[nPoints];
	for (int i = 0; i < nPoints; i++){
		rects[i] = {x, y, w, h};
		if (i == level){
			cursor = {x, y-2, w * 2, h+4};
		}
		x+= pointSpacing + w;
	}
	SDL_RenderFillRects(ctx.renderer, rects, nPoints);

	SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(ctx.renderer, &cursor);
}

int Visuals::createText(std::string text, SDL_Color color, int x, int y, bool behindCamera)
{
	if (!behindCamera){
		std::cout << "DEBUG: Drawing text in front of camera currently not supported" << std::endl;
		return - 1;
	}

	SDL_Surface* surface = TTF_RenderText_Solid(gFont, text.c_str(), color);
	if (surface != NULL){
		auto texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
		if (texture != NULL){
			std::cout << "DEBUG loaded text " << text << std::endl;
			texts.insert({curTextIndex ++, {x, y, surface->w, surface->h, texture, true}});
			return curTextIndex - 1;
		} else {
			std::cout << "DEBUG: Unable to create text texture: " << SDL_GetError() << std::endl;
		}
	} else {
		std::cout << "DEBUG: Unable to create text surface: " << SDL_GetError() << std::endl;
	}
	return -1;
}

void Visuals::renderTexts()
{
	for (auto& text : texts){
		if (text.second.display){
			SDL_Rect pos = {text.second.x, text.second.y, text.second.w, text.second.h};
			//std::cout << "DEBUG: Render text at " << pos.x << "," << pos.y << "," << pos.w << "," << pos.h <<std::endl;
			if (SDL_RenderCopy(ctx.renderer, text.second.texture, NULL, &pos) < 0){
				std::cout << "Failed to render sprite text" << std::endl;
			}
		}
	}
}

void Visuals::destroyAllTexts()
{
	for (auto& text : texts){
		SDL_DestroyTexture(text.second.texture);
	}
	texts = {};
}

void Visuals::renderGameOver()
{
	SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
	SDL_Rect rect = {0, 0, SCREEN_W, SCREEN_H};
	SDL_RenderFillRect(ctx.renderer, &rect);
	if (gameOverTextIndex < 0){
		gameOverTextIndex = createText("You died - press space to start over", {255, 0, 255, 255}, SCREEN_W / 4, SCREEN_H / 2 );
	}
	texts[gameOverTextIndex].display = true;
}
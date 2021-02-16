#include <iostream>
#include <dirent.h>
#include "visuals.hpp"
#include "living_entity.hpp"
#include <iostream>
#include <string.h>

//TODO: place in utils file
void strip_ext(char *fname)
{
    char *end = fname + strlen(fname);

    while (end > fname && *end != '.' && *end != '\\' && *end != '/') {
        --end;
    }
    if ((end > fname && *end == '.') &&
        (*(end - 1) != '\\' && *(end - 1) != '/')) {
        *end = '\0';
    }  
}

bool ends_with(char* haystack, char* needle)
{
	return (strlen(haystack) > strlen(needle) && !strcmp(haystack + strlen(haystack) - strlen(needle), needle));
}

Visuals::Visuals()
{
  	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);

    ctx.renderer = renderer;
    ctx.iteration = 0;


	// if (!initSDL()){
	// 	exit(1);
	// }

	// defaultSpritesheetPath = "./assets/spritesheets";
	// loadSpritesheets(defaultSpritesheetPath);

	// camera = std::make_unique<Camera>(200, 0, 900, 700);
}

Visuals::~Visuals()
{
	for (auto sheet : spritesheets){
		SDL_DestroyTexture(sheet.second);
	}
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
	} else {
		std::cerr << "Can't load spritesheets " << strerror(errno) << std::endl;
		return false;
	}

}

bool Visuals::loadSpritesheet(std::string name)
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

bool Visuals::createWindow(std::string title)
{
    window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN );
        if( window == NULL ){
            std::cerr <<  "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
	return true;
}

SDL_Texture* Visuals::loadTexture( std::string path) const
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cerr << sprintf("Graphics: Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else{
        // 0x64, 0x64, 0x64 is color that will be used for transparency because it's somewhat easy to remember
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x64, 0x64, 0x64));
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        if (newTexture == NULL) {
            std::cerr << sprintf("Graphics: Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}


void Visuals::renderStart()
{
	SDL_SetRenderDrawColor( renderer, 148, 62, 62, 255 );
    SDL_RenderClear( renderer );
}

void Visuals::renderEnd()
{
	SDL_RenderPresent(renderer);
}

void Visuals::renderEntity(Entity* entity)
{
	if (!camera->inView(entity->pos.x,entity->pos.y,entity->pos.w,entity->pos.h))
		return;
	if (entity->type == ITEM && ((Item*)entity)->owner){
		/* When item is owned, owning entity takes care of drawing, easier to handle items in inventory (not draw) + z logic.. */
		return;
	}
 	entity->graphic->render(renderer, camera.get());
}

void Visuals::renderRect(int x, int y, int w, int h)
{
	if (!camera->inView(x, y, w, h))
		return;
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect rect = {x - camera->camRect.x, y - camera->camRect.y, w, h};
	SDL_RenderDrawRect(renderer, &rect);
}

void Visuals::renderRectOverlay(int x, int y, int w, int h)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderDrawRect(renderer, &rect);
}

void Visuals::renderGolfMeter(GolfState state, int level, int nPoints)
{
	// std::cout << "DEBUG render golf meter" << std::endl;
	// SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

	if (state == AIMING_POWER){
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	} else if (state == AIMING_HEIGHT){
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
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
	SDL_RenderFillRects(renderer, rects, nPoints);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &cursor);

}
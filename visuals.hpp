#ifndef __VISUALS_HPP__
#define __VISUALS_HPP__

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>

#define SCREEN_W 920
#define SCREEN_H 640

class Sprite 
{
	public:
		Sprite(SDL_Texture* spritesheet, SDL_Rect src, SDL_Rect pos);
		SDL_Texture* spritesheet;
		SDL_Rect src;
		SDL_Rect pos;
};

class Visuals
{
	public:
		Visuals();
		~Visuals();
		void renderSprite(Sprite* sprite);
		void render();
		SDL_Texture* getSpritesheet(std::string);
		int addSprite(Sprite* sprite);
	private:
		bool initSDL();
		bool loadSpritesheets(std::string dir);
		bool loadSpritesheet(std::string name);
		bool createWindow(std::string title);
		const int renderText(std::string text, SDL_Color color, int x, int y, int h) const;
		SDL_Texture* loadTexture( std::string path) const;
		std::unordered_map<std::string, SDL_Texture*> spritesheets;
		std::vector<Sprite*> sprites;

		std::string defaultSpritesheetPath = ".";

		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
};

#endif
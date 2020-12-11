#ifndef __VISUALS_HPP__
#define __VISUALS_HPP__

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include "sprite.hpp"

#define SCREEN_W 920
#define SCREEN_H 640

class Visuals
{
	public:
		Visuals();
		~Visuals();
		void renderStart();
		SDL_Texture* getSpritesheet(std::string);
		void renderEnd();
		void combineTextures();
		void renderSprite(Sprite* sprite);
		void renderRect(int x, int y, int w, int h);

	private:
		bool initSDL();
		bool loadSpritesheets(std::string dir);
		bool loadSpritesheet(std::string name);
		bool createWindow(std::string title);
		const int renderText(std::string text, SDL_Color color, int x, int y, int h) const;
		SDL_Texture* loadTexture( std::string path) const;
		std::unordered_map<std::string, SDL_Texture*> spritesheets;

		std::string defaultSpritesheetPath = ".";

		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
};

#endif
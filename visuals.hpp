#ifndef __VISUALS_HPP__
#define __VISUALS_HPP__

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <memory>
#include "golf_mode.hpp"
#include "graphic.hpp"
#include "camera.hpp"
#include "entity.hpp"

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
		void renderEntity(Entity* entity);
		void renderRect(int x, int y, int w, int h);
		// Overlay implies 'behind' the camera
		void renderRectOverlay(int x, int y, int w, int h);
		void renderGolfMeter(GolfState state, int n);
		std::unique_ptr<Camera> camera;
		std::unordered_map<std::string, SDL_Texture*> spritesheets;

	private:
		bool initSDL();
		bool loadSpritesheets(std::string dir);
		bool loadSpritesheet(std::string name);
		bool createWindow(std::string title);
		const int renderText(std::string text, SDL_Color color, int x, int y, int h) const;
		SDL_Texture* loadTexture( std::string path) const;

		std::string defaultSpritesheetPath = ".";

		SDL_Window* window;
		SDL_Renderer* renderer;
		TTF_Font* font;
};

#endif
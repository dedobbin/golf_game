#ifndef __VISUALS_HPP__
#define __VISUALS_HPP__

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <memory>
#include "golf_mode.hpp"
#include "graphic.hpp"
#include "camera.hpp"
#include "entity.hpp"

#define SCREEN_W 920
#define SCREEN_H 640

struct context
{
    SDL_Renderer *renderer;
    int iteration;
};

class Visuals
{
	public:
		Visuals();
		~Visuals();
		SDL_Texture* getSpritesheet(std::string);
		void renderStart();
		void renderEnd();
		void renderEntity(Entity* entity);
		void renderRect(int x, int y, int w, int h);
		void renderGameOver();
		// Overlay implies 'behind' the camera
		void renderRectOverlay(int x, int y, int w, int h);
		bool loadSpritesheets(std::string dir);
		void loadSpritesheet(std::string name);
		void renderGolfMeter(GolfState state, int level, int nPoints);
		std::unique_ptr<Camera> camera;
		const std::string defaultSpritesheetPath = "./assets/spritesheets";
		std::unordered_map<std::string, SDL_Texture*> spritesheets;
		context ctx;

	private:
		SDL_Texture* loadTexture( std::string path) const;
};

#endif
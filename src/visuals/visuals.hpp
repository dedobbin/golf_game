#ifndef __VISUALS_HPP__
#define __VISUALS_HPP__

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <memory>
#include "../entity_properties/golf_mode.hpp"
#include "../entity_properties/graphic.hpp"
#include "../entities/entity.hpp"
#include "camera.hpp"

#define SCREEN_W 920
#define SCREEN_H 640

#ifdef __EMSCRIPTEN__
	#define RESOURCES_PATH "resources"
#else
	#define RESOURCES_PATH "../resources"
#endif

extern int gameOverTextIndex;

struct context
{
    SDL_Renderer *renderer;
    int iteration;
};

struct Text
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	SDL_Texture* texture = NULL;
	SDL_Color color = {255, 0, 255, 255};
	bool display = true;
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
		// Overlay implies 'behind' the camera
		void renderRectOverlay(int x, int y, int w, int h);
		void renderGolfMeter(GolfState state, int level, int nPoints);

		/* returns ID */
		int createText(std::string text, int x, int y, SDL_Color color = {255, 0, 255, 255}, bool behindCamera = true);
		void renderTexts();
		void renderGameOver();
		void destroyText(int textId);
		void destroyAllText();
		void updateText(std::string text, int textId);

		bool loadSpritesheets(std::string dir);
		void loadSpritesheet(std::string name);
		std::unique_ptr<Camera> camera;
		const std::string defaultSpritesheetPath = RESOURCES_PATH + std::string("/spritesheets");
		std::unordered_map<std::string, SDL_Texture*> spritesheets;
		context ctx;
		TTF_Font* gFont = NULL;

	private:
		SDL_Texture* loadTexture( std::string path) const;
		std::unordered_map<int, Text> texts;
};

#endif
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include <string>

class ColliderComponent;

class Game
{
public:

	Game();
	Game(char*, int, int, int, int, bool);
	~Game();

	void Init(const char* title, int xPos, int yPos, int width, int height, bool fullScr);
	
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
	void InstantiatePlayer();
	void InstantiateWall();

	static void AddTile(int srcX, int srcY, int xPos, int yPos);

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;

	bool Running() { return isRunning; };

private:

	int framesRendered = 0;
	bool isRunning = false;
	SDL_Window *window;
};


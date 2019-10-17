#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include <string>
#include "Vector2D.h"

class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	enum OriginType : std::size_t {
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		LEFT,
		CENTERED,
		RIGHT,
		BOTTOM_LEFT,
		BOTTOM,
		BOTTOM_RIGHT
	};

	void Init(const char* title, int xPos, int yPos, int width, int height, bool fullScr);
	
	void Clean();
	void HandleEvents();
	void Render();
	bool Running() { return isRunning; };
	void Update();

	static void AddTile(int srcX, int srcY, int xPos, int yPos);
	//static Vector2D CalculateOffset(int originType);

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static bool isRunning;

	static SDL_Rect camera;

	void InstantiatePlayer();
	void InstantiateWall();
	void UpdateCamera();
	void HandleMouse();

	void InstantiateEmitter();
	void InstantiateObjectOfTypeAt(int objectType, int x, int y, Vector2D velocity, int origin);
	void InstantiateParticle(int x, int y, Vector2D velocity, int origin);
	
private:

	Vector2D mouseCoords;
	Uint32 mouseState;
	int framesRendered = 0;
	int particles = 0;
	SDL_Window *window;
	bool mouseClick = false;
};

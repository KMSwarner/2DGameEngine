#include <time.h>
#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "../GameEngine2D_rev2/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;
Manager manager;


SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0,0,0,0 };

std::vector <ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

const char* mapFile = "assets/Maps/terrain_ss.png";

enum objectType : std::size_t {
	objectDummy,
	objectEmitter,
	objectProjectile,
	objectParticle
};

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupNeutrals,
	groupColliders,
	groupProjectiles
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& neutrals(manager.getGroup(groupNeutrals));
auto& colliders(manager.getGroup(groupColliders));
auto& projectiles(manager.getGroup(groupProjectiles));
//auto& tiles(manager.getGroup(groupColliders));

Game::Game()
{
	Game::Init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, false);
}

Game::~Game()
{
	
}

void Game::Init(const char* title, int xPos, int yPos, int wWidth, int wHeight, bool fScr)
{
	int flags = 0;

	std::srand((unsigned)time(NULL));

	camera.w = wWidth;
	camera.h = wHeight;

	if (fScr)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized." << std::endl;

		window = SDL_CreateWindow(title, xPos, yPos, wWidth, wHeight, flags);
		if (window)
		{
			std::cout << "Window created successfully." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created successfully." << std::endl;
		}

		isRunning = true;
	}

	map = new Map();

	Map::LoadMap("assets/Maps/map.map", 25, 25);

	InstantiatePlayer();
	//InstantiateWall();
}

void Game::InstantiatePlayer()
{
	player.addComponent<TransformComponent>(50,50,24,26,4,4,TOP_LEFT);
	//player.addComponent<AnimatedSpriteComponent>("assets/Yellow.png", 8, 8, 1);
	player.addComponent<SpriteComponent>("assets/Yellow_anims.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addToGroup(groupPlayers);
}

void Game::InstantiateWall()
{
	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1, 1, TOP_LEFT);
	wall.addComponent<SpriteComponent>("assets/dirt.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addToGroup(groupMap);
}

/*
 Velocity to be provided in terms of polar coordinates. (+/-Angle/360deg, magnitude)
*/

/*Start working out particle system from this:
	Adding a self-destruct component lends itself to particle behavior.
*/
void Game::InstantiateObjectOfTypeAt(int objectType, int x, int y, Vector2D velocity, int origin)
{
	Vector2D position = Vector2D(x + camera.x, y + camera.y);
	Vector2D dimensions = Vector2D(24, 26);
	Vector2D scale = Vector2D(2, 2);
	Vector2D offset;
	float expiryTime{};

	auto& newEntity(manager.addEntity());

	//Set values according to object types
	switch (objectType)
	{
	case objectDummy:
		newEntity.addComponent<TransformComponent>(position, dimensions, scale, origin);
		newEntity.addComponent<SpriteComponent>("assets/player.png", false);
		newEntity.addToGroup(groupEnemies);
		newEntity.setTag("dummy");
		break;

	//Projectiles and particles need to be spawned with a relative offset to track properly.
	//Offset should be relative to the camera, being converted to absolute position on the map.
	case objectEmitter:
		expiryTime = 0.0;
		newEntity.addComponent<TransformComponent>(position, dimensions, scale, origin);
		newEntity.addComponent<ParticleSystemComponent>(this, 1.0f, expiryTime, 0, ParticleSystemComponent::Small_Bubbles); //Reference to game, particles every second/1, emission duration in seconds, emitter type
		newEntity.addComponent<SelfDestructComponent>(expiryTime); //Self destruct after n seconds
		newEntity.addComponent<SpriteComponent>("assets/player.png", false);
		newEntity.addToGroup(groupEnemies);
		newEntity.setTag("emitter");
		break;

	case objectProjectile:
		newEntity.addComponent<TransformComponent>(position, dimensions, scale, origin);
		//newEntity.getComponent<TransformComponent>().speed = velocity.y;
		//newEntity.getComponent<TransformComponent>().XYFromDegrees(std::rand() % 360);
		newEntity.addComponent<SpriteComponent>("assets/player.png", false);
		newEntity.addToGroup(groupEnemies);
		newEntity.setTag("projectile");

	default:
		break;
	}
}

void Game::InstantiateEmitter()
{
	//Emitter needs to know where it is meant to instantiate and what type it should be
	//Using the assigned type, chose from the list below what particle functions to place in the list
	//Pass that list to the particle itself

	//This may be too heavyweight to attach to every particle. Emitter should hold "playlist" of functions and
	//should access a managed list of instantiated particles
}

//Other way to do particles: Have one emitter that instantiates and tracks many sprites.
//No entity assignment necessary.
//Create and delete SDL_Rects as needed.
//Put rects into a list
//Modify rects using necessary functions
//Delete when they've reached a base case (minimum distance achieved, time limit, etc)

void Game::InstantiateParticle(int x, int y, Vector2D velocity, int origin)
{
	Vector2D position = Vector2D(x + camera.x, y + camera.y);
	Vector2D dimensions = Vector2D(24, 26);
	Vector2D scale = Vector2D(2, 2);
	Vector2D offset;

	auto& newEntity(manager.addEntity());

	newEntity.setTag("particle");
	newEntity.addComponent<TransformComponent>(x, y, 24, 26, 2, 2, origin);
	newEntity.getComponent<TransformComponent>().speed = velocity.y;
	newEntity.getComponent<TransformComponent>().XYFromDegrees(std::rand() % 360);
	newEntity.addComponent<SpriteComponent>("assets/player.png", false);
	newEntity.addComponent<SelfDestructComponent>(10.0f);
	newEntity.addToGroup(groupNeutrals);
}

void Game::HandleEvents()
{
	//This is a change made on Oct. 12, 2019 to improve the function of the event handler.
	//
	//Previously, once an event occurred, that event would be processed continuously
	//until another took its place.
	//
	//In the case of mouse events, the last button event would continue to fire, until
	//movement or another button click replaced it.
	//
	//E.g.: A left button up would continuously fire until it was replaced by any
	//other mouse event.
	//
	//Possible connection with key inputs for player motion causing stalling or carry-over
	//when a key is released after being held for long periods.
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		//If the event type is a key release...
		case SDL_KEYUP:
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			default:
				break;
			}

		//If the event has anything to do with the mouse, pass it on
		case SDL_MOUSEBUTTONUP:
			HandleMouse();
			break;

		case SDL_MOUSEBUTTONDOWN:
			HandleMouse();
			break;

		case SDL_MOUSEWHEEL:
			HandleMouse();
			break;

		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
		}
	}
}

//Old update function, "removed" during addition of culling behavior
//void Game::Update()
//{
//	manager.Refresh();
//	manager.Update();
//
//	Vector2D pVel = player.getComponent<TransformComponent>().velocity;
//	int pSpeed = player.getComponent<TransformComponent>().speed;
//
//	//Disabled during "static player / scrolling background" rework
//	/*for (auto cc : colliders)
//	{
//		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
//	}*/
//
//	//This seems very clumsy and unnecessarily resource heavy.
//	for (auto t : tiles)
//	{
//		t->getComponent<TileComponent>().destRect.x -= (pVel.x * pSpeed);
//		t->getComponent<TileComponent>().destRect.y -= (pVel.y * pSpeed);
//	}
//}

void Game::Update()
{
	manager.Refresh();
	manager.Update();
	UpdateCamera();
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->Draw();
	}
	for (auto& p : players)
	{
		p->Draw();
	}
	for (auto& e : enemies)
	{
		e->Draw();
	}
	for (auto& n : neutrals)
	{
		n->Draw();
	}
	for (auto& c : colliders)
	{
		c->Draw();
	}for (auto& p : projectiles)
	{
		p->Draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Game cleaned." << std::endl;
}

void Game::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	auto& tile(manager.addEntity());
	//tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, mapFile);
	tile.addToGroup(groupMap);
}

void Game::UpdateCamera()
{
	//Defining camera position relative to player, centering player within view.
	camera.x = player.getComponent<TransformComponent>().position.x - (camera.w / 2);
	camera.y = player.getComponent<TransformComponent>().position.y - (camera.h / 2);

	if (camera.x < 0)
	{
		camera.x = 0;
	}

	if (camera.y < 0)
	{
		camera.y = 0;
	}

	//This only makes sense if the game board is 2x the window size. Non-reusable!!
	if (camera.x > (camera.w) * 2)
	{
		camera.x = camera.w;
	}
	//This only makes sense if the game board is 2x the window size. Non-reusable!!
	if (camera.y > camera.h)
	{
		camera.y = camera.h;
	}
}

void Game::HandleMouse()
{
	int x = 0;
	int y = 0;
	float wheelY = static_cast<float>(event.wheel.y);

	SDL_GetMouseState(&x, &y);
	mouseCoords.x = x;
	mouseCoords.y = y;

	//Right now, this is checking if the LAST mouse event was a button up. Meaning if the mouse is not moved, if no button is clicked,
	//and there is no other mouse event whatsoever, that event stays in the queue and doesn't clear until the next comes along.
	//MUST find a way to clear it to prevent multiple clicks registering for a single one.
	switch (event.type)
	{
		//Button up events
	case SDL_MOUSEBUTTONUP:
		switch (event.button.button)
		{
			//Left up
		case SDL_BUTTON_LEFT:
			std::cout << "Left up at: " << x << ", " << y << std::endl;
			//InstantiateObjectAt(objectDummy, x, y, Vector2D(0,0), true);
			InstantiateObjectOfTypeAt(objectEmitter, x, y, Vector2D(0, 3), CENTERED);
			break;

			//Right up
		case SDL_BUTTON_RIGHT:
			std::cout << "Right up" << std::endl;

			break;
			//Middle up
		case SDL_BUTTON_MIDDLE:
			std::cout << "Mid up" << std::endl;
			break;

		default:
			break;
		}
		break;

		//Button down events
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button)
		{
			//Left up
		case SDL_BUTTON_LEFT:
			std::cout << "Left down" << std::endl;
			break;

			//Right up
		case SDL_BUTTON_RIGHT:
			std::cout << "Right down" << std::endl;
			break;

			//Middle up
		case SDL_BUTTON_MIDDLE:
			std::cout << "Mid down" << std::endl;
			break;

		default:
			break;
		}
		break;

	case SDL_MOUSEWHEEL:

		if (wheelY < 0)
		{
			std::cout << "Scroll down: " << wheelY << std::endl;
		}

		else if (wheelY > 0)
		{
			std::cout << "Scroll up" << wheelY << std::endl;
		}
		else
		{
			std::cout << "Scroll unknown: " << wheelY << std::endl;
		}

		break;

	default:
		break;
	}
}
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

std::vector <ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

const char* mapFile = "assets/Maps/terrain_ss.png";

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};


Game::Game()
{
	Game::Init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, false);
}

Game::Game(char* title, int xPos, int yPos, int width, int height, bool fScr)
{
	Game::Init(title, xPos, yPos, width, height, fScr);
}


Game::~Game()
{
	
}

void Game::Init(const char* title, int xPos, int yPos, int width, int height, bool fScr)
{
	int flags = 0;

	if (fScr)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialized." << std::endl;

		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
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
	player.addComponent<TransformComponent>(50,50,24,26,4,4);
	//player.addComponent<AnimatedSpriteComponent>("assets/Yellow.png", 8, 8, 1);
	player.addComponent<SpriteComponent>("assets/Yellow_anims.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addToGroup(groupPlayers);
}

void Game::InstantiateWall()
{
	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1, 1);
	wall.addComponent<SpriteComponent>("assets/dirt.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addToGroup(groupMap);
}

void Game::HandleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
		isRunning = false;
			break;

		default:
			break;
	}
}

void Game::Update()
{
	manager.Refresh();
	manager.Update();

	for (auto cc : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
	}
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
//auto& tiles(manager.getGroup(groupColliders));

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
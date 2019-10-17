#pragma once
#include "ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	Vector2D positionInWorld;
	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, const char* path)
	{
		texture = TextureManager::LoadTexture(path);

		//True position in world
		positionInWorld.x = xPos;
		positionInWorld.y = yPos;

		//Location in sprite sheet
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = 32;

		//Position on screen, relative to camera
		destRect.x = xPos;
		destRect.y = yPos;
		destRect.w = destRect.h = 64;
	}

	TileComponent(Vector2D &pos, Vector2D &dim, int ID)
	{
	}

	void Update() override
	{
		destRect.x = positionInWorld.x - Game::camera.x;
		destRect.y = positionInWorld.y - Game::camera.y;
	}

	void Draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};
#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y)
{
	objTex = TextureManager::LoadTexture(textureSheet);

	xPos = x;
	yPos = y;
}

GameObject::~GameObject()
{
}

void GameObject::Awake()
{

}

void GameObject::Start()
{

}

void GameObject::Update()
{
	xPos++;
	yPos++;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.h = 24;
	srcRect.w = 24;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.h = srcRect.h * 2;
	destRect.w = srcRect.h * 2;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objTex, &srcRect, &destRect);

}
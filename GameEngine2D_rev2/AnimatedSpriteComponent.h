#pragma once
#include "TransformComponent.h"
#include "SDL.h"
#include "TextureManager.h"

class AnimatedSpriteComponent : public Component
{
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	int currentFrame;
	int frames;
	int cols;
	int rows;

public:
	AnimatedSpriteComponent() = default;
	AnimatedSpriteComponent(const char* path, int frameCount, int c, int r)
	{
		frames = frameCount;
		setTex(path);
		cols = c;
		rows = r;
	}

	~AnimatedSpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void Init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = static_cast<int>(transform->dimensions.x);
		srcRect.h = static_cast<int>(transform->dimensions.y);

		destRect.w = static_cast<int>(transform->dimensions.x * transform->scale.x);
		destRect.h = static_cast<int>(transform->dimensions.y * transform->scale.y);

		currentFrame = 0;
	}

	void Update() override
	{
		currentFrame = (1 + currentFrame) % frames; //Advances the frame and resets to 0 if we've run through all

		srcRect.x = (srcRect.w * (currentFrame % cols));
		srcRect.y = (srcRect.h * (currentFrame / cols));

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);

		destRect.w = static_cast<int>(transform->dimensions.x * transform->scale.x);
		destRect.h = static_cast<int>(transform->dimensions.y * transform->scale.y);
	}

	void Draw() override
	{
		//TextureManager::Draw(texture, srcRect, destRect);
	}



};
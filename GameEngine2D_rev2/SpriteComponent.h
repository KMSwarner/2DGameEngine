#pragma once
#include "TransformComponent.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool canDraw = false;
	bool animated = false;
	int frames = 0;
	int speed = 100; //To be used as delay between frames in milliseconds

public:
	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		setTex(path);
	}

	SpriteComponent(const char* path, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 4, 100); //"Idle"
		Animation walk = Animation(1, 8, 100); //Walk

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);

		Play("Idle");

		setTex(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void Init() override
	{
		//if (entity->getTag() == "particle")
		//	std::cout << "Init particle sprite" << std::endl;

		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = static_cast<int>(transform->dimensions.x);
		srcRect.h = static_cast<int>(transform->dimensions.y);

		destRect.x = transform->position.x;
		destRect.y = transform->position.y;
		destRect.w = static_cast<int>(transform->dimensions.x * transform->scale.x);
		destRect.h = static_cast<int>(transform->dimensions.y * transform->scale.y);

		//This is a blunt work-around. Find a way to fix the real issue, instead of simply skipping the first frame.
		if (entity->getTag() != "particle")
			canDraw = true;
	}

	void Update() override
	{
		//if (entity->getTag() == "particle")
		//	std::cout << "Updating particle sprite";

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames); //Animates independently of game's framerate
		}
		srcRect.y = animIndex * transform->dimensions.y;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;

		destRect.w = static_cast<int>(transform->dimensions.x * transform->scale.x);
		destRect.h = static_cast<int>(transform->dimensions.y * transform->scale.y);
	}

	void Draw() override
	{
		//Somehow, particles are getting a compound translation involving the camera location offset. This error corrects itself on the second frame drawn.
		//FIX IT!!
		if (canDraw)
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
		
		//This is a rough fix to skip drawing on the first frame when the particle is out of place. This will likely cause problems in the future with physical particles.
		if (entity->getTag() == "particle")
			canDraw = true;
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frameCount;
		animIndex = animations[animName].index;
		speed = animations[animName].playbackSpeed;
	}
};
#pragma once
#include "Game.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class KeyboardController : public Component
{
private:
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	void Init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void Update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
				case SDLK_w:
				{
					transform->velocity.y = -1;
					sprite->Play("Walk");
					break;
				}

				case SDLK_a:
				{
					transform->velocity.x = -1;
					sprite->Play("Walk");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				}

				case SDLK_s:
				{
					transform->velocity.y = 1;
					sprite->Play("Walk");
					break;
				}

				case SDLK_d:
				{
					transform->velocity.x = 1;
					sprite->Play("Walk");
					if (sprite->spriteFlip == SDL_FLIP_HORIZONTAL)
					{
						sprite->spriteFlip = SDL_FLIP_NONE;
					}

					break;
				}

				default:
				{
					break;
				}
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
				case SDLK_w:
				{
					transform->velocity.y = 0;
					sprite->Play("Idle");
					break;
				}

				case SDLK_a:
				{
					transform->velocity.x = 0;
					sprite->Play("Idle");
					break;
				}

				case SDLK_s:
				{
					transform->velocity.y = 0;
					sprite->Play("Idle");
					break;
				}

				case SDLK_d:
				{
					transform->velocity.x = 0;
					sprite->Play("Idle");
					break;
				}

				default:
				{
					break;
				}
			}
		}

		//transform->velocity = new Vector2D(0, 0);

		//if (currentKeyStates[SDL_SCANCODE_W])
		//{
		//	transform->velocity.y = -1;
		//}

		//else if (currentKeyStates[SDL_SCANCODE_A])
		//{
		//	transform->velocity.x = -1;
		//}

		//else if (currentKeyStates[SDL_SCANCODE_S])
		//{
		//	transform->velocity.y = 1;
		//}

		//else if (currentKeyStates[SDL_SCANCODE_D])
		//{
		//	transform->velocity.x = 1;
		//}
	}
};
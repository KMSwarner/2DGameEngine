#pragma once
#include <string>
#include "Collision.h"
#include "SDL.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "Game.h"

class ColliderComponent : public Component
{
	TransformComponent* transform;

public:
	SDL_Rect collider;
	std::string tag;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	void Init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this);
	}

	void Update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);

		collider.w = static_cast<int>(transform->dimensions.x * transform->scale.x);
		collider.h = static_cast<int>(transform->dimensions.y * transform->scale.y);
	}

	SDL_Rect getCollider()
	{
		return collider;
	}

	std::string getTag()
	{
		return tag;
	}
};
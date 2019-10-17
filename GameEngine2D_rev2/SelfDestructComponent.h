#pragma once
#include "ECS.h"
#include "Game.h"

class SelfDestructComponent : public Component
{
public:
	float triggerTime{};
	float lastFrame{};
	bool infinite{false};

	SelfDestructComponent();
	SelfDestructComponent(float seconds)
	{
		if(seconds > 0)
			triggerTime = seconds;

		else
		{
			infinite = true;
		}
	}

	~SelfDestructComponent()
	{

	}

	void Init() override
	{
		lastFrame = 0;
	}

	void Update() override
	{
		if (!infinite && triggerTime <= 0)
		{
			//std::cout << "POP!";
			entity->setActive(false);
		}

		triggerTime -= 1.0f/60.0f;
		/*entity->getComponent<TransformComponent>().scale.x -= 1.0f / 60.0f;
		entity->getComponent<TransformComponent>().scale.y -= 1.0f / 60.0f;*/
		lastFrame = SDL_GetTicks();
	}
};
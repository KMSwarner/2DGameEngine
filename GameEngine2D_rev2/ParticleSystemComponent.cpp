#include "ParticleSystemComponent.h"

ParticleSystemComponent::ParticleSystemComponent(Game* gRef, float rate, float time, int effectsList, int emType)
{
	game = gRef;
	emissionRate = rate;

	if (time > 0) //If time <= 0, emission time will be infinite
	{
		emissionTimeLimit = time;
	}
	else
	{
		infinite = true;
		std::cout << "infinite" << std::endl;
	}

	switch (emType)
	{
	case 0:
		//behavior = &ParticleSystemComponent::bubbleBehavior;
		functionVector.push_back(&ParticleSystemComponent::smallRisingBubbleBehavior);
		//functionVector.push_back(&ParticleSystemComponent::dummyBehavior);
		break;

	default:
		break;
	}
}

ParticleSystemComponent::~ParticleSystemComponent()
{
}

void ParticleSystemComponent::Init()
{
	lastEmit = 0.0f;

	//entity->getComponent<TransformComponent>().XYFromDegrees(emissionVelocity.x);
	//entity->getComponent<TransformComponent>().speed = emissionVelocity.y;
}

void ParticleSystemComponent::Draw()
{
	for (ParticleModel p : particleList)
	{
		TextureManager::Draw(p.tex, p.sourceRect, p.destRect, SDL_FLIP_NONE);
	}
}

void ParticleSystemComponent::Update()
{
	lastEmit += 1.0 / 60.0;

	if (emissionTimeLimit >= 0 || infinite)
	{
		if (!infinite)
		{
			emissionTimeLimit -= 1.0 / 60.0;
		}

		if (lastEmit > emissionRate)
		{
			lastEmit = 0;

			Emit();
		}

		//If there are any particles to modify, ...
		if (!particleList.empty())
		{
			runFunctionList();
			cleanParticlesList();
		}
	}
}

void ParticleSystemComponent::runFunctionList()
{
	//For every function in the list, ...
	for (int i{}; i < functionVector.size(); i++)
	{
		//... call that function.
		(this->*functionVector[i])();
	}
}

void ParticleSystemComponent::cleanParticlesList()
{
	//Update their remaining lifespan and...
	std::list<ParticleModel>::iterator p = particleList.begin();
	while (p != particleList.end())
	{
		//... if they're spent...
		if (p->checkLife())
		{
			//... remove it from the list.
			particleList.erase(p++);
		}
		else
		{
			++p;
		}
	}
}

void ParticleSystemComponent::smallRisingBubbleBehavior()
{
	for (ParticleModel& p : particleList)
	{
		int deltaPixels;
		float drift;
		float rise;
	
		deltaPixels = entity->getComponent<TransformComponent>().position.y - p.destRect.y;

		//rise = easeIn(deltaPixels+0.5, 2);

		//p.destRect.y -= 2 * ((rand() % 100) / 10);
		p.destRect.y -= 2;

		drift = normalizedSin(deltaPixels, 0.05, 2.5);

		p.destRect.x = entity->getComponent<TransformComponent>().position.x + drift * 10;
	}
}

void ParticleSystemComponent::dummyBehavior()
{

	std::cout << "Dummy function" << std::endl;
}

void ParticleSystemComponent::Emit()
{
	const float x{ entity->getComponent<TransformComponent>().position.x };
	const float y{ entity->getComponent<TransformComponent>().position.y };

	ParticleModel p(Vector2D(x, y), emissionVelocity, Vector2D(16, 16), 1, 2);
	p.tex = TextureManager::LoadTexture("assets/player.png");

	p.Init();

	particleList.emplace_back(p);
}
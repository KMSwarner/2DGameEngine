#pragma once
#include "ECS.h"
#include "Game.h"
#include "Components.h"
#include <list>
#include "ParticleModel.h"
#include "TextureManager.h"

class ParticleSystemComponent : public Component
{
private:
	float emissionRate{};
	float lastEmit{};
	float emissionTimeLimit{};

	const char* fileName;

	bool infinite {false};

	float(*pFunc)(float, float);
	void(ParticleSystemComponent::*behavior)(std::list<ParticleModel> &particles);

	Vector2D emissionVelocity;
	std::list<ParticleModel> particleList;
	std::vector<void(ParticleSystemComponent::*)()> functionVector;

	Game* game;

public:
	enum {
		
		Small_Bubbles,
		Embers
	};

	ParticleSystemComponent(Game* gRef, float rate, float time, int effectsList, int emType);
	~ParticleSystemComponent();

	void Update() override;

	void Emit();

	void Init() override;

	void Draw() override;

	void cleanParticlesList();

	void runFunctionList();

	void smallRisingBubbleBehavior();

	void dummyBehavior();

	float normalizedSin(float x, float freq, float amp)
	{
		float y{};

		y = sin(x * (freq / M_PI));

		return y * amp;
	}

	float EasingCurve(float start, float end, float currentX, float coeff_k, float multiplier, int Curve_Type)
	{
		float y{};
		int k = coeff_k;
		float x = currentX;
		float m = multiplier;

		switch (Curve_Type)
		{
		case 1: //Constant. Y = X
			return currentX;
		case 2: //Linear growth, add
			return x + k;
		case 3: //Linear growth, mult
			return x * k;
		case 4: //Exponential. K serves as power
			return power(x, k);
		case 5://Ease both; k acts as the +/-max amplitude
			return normalizedSin(x, m, k);
		case 6: //Ease-in
			return easeIn(x, k);
		case 7: //Ease-out
			return easeOut(x, k);
		case 8: //Sharp ease-in
			return sharpEaseIn(x, k);
		case 9: //Sharp ease-out
			return sharpEaseOut(x, k);
		case 10: //Ease both
			return easeBoth(x, k);
		}

		return y;
	}

	float power(float x, float k)
	{
		float y{ 1.0 };

		for (int i{ 1 }; i <= k; i++)
		{
			y *= x;
		}

		return y;
	}

	//Returns a normalized result between 0 and 1*amp
	float easeBoth(float x, float amp)
	{
		float y{};

		y = cosf((x * 2 * M_PI) + M_PI) + 1;
		return y * (amp / 2.0);

	}

	//Steadily ease in on parabolic curve from 0.0 to 1.0
	float easeIn(float x, float k)
	{
		float y{};

		y = sinf(x * (M_PI / 2));

		return y * k;
	}

	//Rises quickly from 0.0 to ~0.5, then slowly approaches 1.0
	float sharpEaseIn(float x, float k)
	{
		float y{};

		y = tanhf(x * M_PI);
		return y * k;
	}

	//Steadily ease out on parabolic curve from 1.0 to 0.0
	float easeOut(float x, float k)
	{
		float y{};

		y = -sinf(x * (M_PI / 2)) + 1;

		return y * k;
	}

	//Falls quickly from 1.0 to ~0.5, then slowly approaches 0
	float sharpEaseOut(float x, float k)
	{
		float y{};

		y = (tanhf(x * M_PI) - 1);
		return y * -k;
	}
};


/*
//Create a list of function pointers to act as an effects list. Functions will exist as part of the component, but only those selected
//for a specific emitter will be called.
//std::list<function*> effectsList;

//Can these functions exist in a separate header as static, and be passed that way, or must they be a part of the ParticleComponent class and be enqueued in that way?
//If they exist in the component header, is it viable to incorporate an array of the available functions and simply pass a list/array/vector/whatever of the indices?
//Note: should also be able to pass list of parameters for chosen functions. This could get complex...

//Build the arrays separately, then pass them.


//creating and retrieving effects lists:
//In constructor:
//		take in int designating the library index of effects lists. (Each index will point to the list of effects for this particle. Those effects are indices into the function pointer effects list)
//		These lists also consist of integers as indicies for their respective effects.


// Create a library of values for effect type and parameters:
//			If this is a movement function, what is my target velocity?
//			If this is a scaling function, how small/large and how quickly?

//Constructor( receive index into scripted effects sequence library ) --->> EffectsListBuilder( use library index to find row designating functions and add them to function pointers list, build array of values
//for each function, using the function count to set the array size. Create double array with enough rows for the queued functions. Make each array row large enough for that function's parameters list.
//Fill each array row.)
*/


//Instantiate particle model
//Manipulate each as necessary
//Pass in the texture assigned to the emitter, the source rect for the texture, the destination rect of the particle using the particle's uniform scale.
//SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip); //Extra features, including pivot point, angle, flip


/*
	void Update() override
	{
		//std::cout << "Particle pos before update: x = " << entity->getComponent<TransformComponent>().position.x << " y = " << entity->getComponent<TransformComponent>().position.y << std::endl;

		int x, y;

		lastEmit += 1.0 / 60.0;

		if (emissionTimeLimit >= 0 || emissionInfinite)
		{
			if (!emissionInfinite)
			{
				emissionTimeLimit -= 1.0 / 60.0;
			}

			if (lastEmit > emissionRate)
			{
				x = entity->getComponent<TransformComponent>().position.x;
				y = entity->getComponent<TransformComponent>().position.y;

				//Using default origin pos of T_L so that particles spawn centered with emitter. Else, causes compounded offset
				//							x		y		   (random angle        speed)     origin alignment
				game->InstantiateParticle(x, y, Vector2D(std::rand() % 360, 3), Game::TOP_LEFT);
				lastEmit = 0;
			}
		}

		//std::cout << "Particle pos after update: x = " << entity->getComponent<TransformComponent>().position.x << " y = " << entity->getComponent<TransformComponent>().position.y << std::endl;
	}*/

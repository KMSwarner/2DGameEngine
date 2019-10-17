#pragma once
#include "Vector2D.h"
#include "SDL.h"

class ParticleModel
{
private:
	double uniformScale;		//Used to scale x and y dimensions uniformly

public:
	Vector2D pos;			//X and Y positioning of the particle
	Vector2D velocity;		//Angle and speed of the particle. Polar coordinates.
	Vector2D dimensions;	//Height and Width of the particle

	float lifeSpan;			//In seconds

	Uint32 lastTicks;

	SDL_Rect sourceRect;
	SDL_Rect destRect;
	SDL_Texture* tex;

	ParticleModel(Vector2D p, Vector2D v, Vector2D d, double uScale, float span)
	{
		pos = p;

		sourceRect.x = 0;
		sourceRect.y = 0;
		sourceRect.w = 24;
		sourceRect.h = 26;

		destRect.x = p.x;
		destRect.y = p.y;

		velocity = v;

		destRect.w = d.x;
		destRect.h = d.y;

		uniformScale = uScale;

		lifeSpan = span;
	}

	~ParticleModel()
	{
	}

	void Init()
	{
		lastTicks = SDL_GetTicks();
	}

	bool checkLife()
	{
		Uint32 thisTicks = SDL_GetTicks();

		if (thisTicks != lastTicks)
		{
			if (lifeSpan <= 0)
			{
				return true;
			}
			lifeSpan -= 1.0 / 60.0;
		}
		return false;
	}

	bool operator==(const ParticleModel& rhs)
	{
		return this == &rhs;
	}

};
#pragma once
#include "ECS.h"
#include "Vector2D.h"


class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D scale;
	Vector2D dimensions;
	Vector2D velocity;

	int speed;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int x, int y, int w, int h, float scaleX, float scaleY)
	{
		position.x = (float)x;
		position.y = (float)y;

		dimensions.x = (float)w;
		dimensions.y = (float)h;

		scale.x = (float)scaleX;
		scale.y = (float)scaleY;

		speed = 3;
	}

	TransformComponent(Vector2D pos, Vector2D dim, Vector2D sca)
	{
		position = pos;
		dimensions = dim;
		scale = sca;
	}

	int x() { return (int)position.x; }
	int y() { return (int)position.y; }

	void Init() override
	{
		velocity.Zero();
	}

	void setPos(Vector2D pos)
	{
		position = pos;
	}

	void setPos(int x, int y)
	{
		position.x = (float)x;
		position.y = (float)y;
	}

	void setScale(Vector2D sca)
	{
		scale = sca;
	}

	void setScale(float x, float y)
	{
		scale.x = x;
		scale.y = y;
	}

	void setDimensions(Vector2D dim)
	{
		dimensions = dim;
	}

	void setDimensions(int w, int h)
	{
		dimensions.x = (float)w;
		dimensions.y = (float)h;
	}

	void Update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};

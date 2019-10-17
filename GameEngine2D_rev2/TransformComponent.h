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
	Vector2D origin;

	int speed;

	TransformComponent()
	{
		position.Zero();
	}

	//This constructor only used by player? Don't like it. Non-reusable.
	//Has been adapted to facilitate player center-of-screen with scrolling background.
	//Eliminate to revert.
	TransformComponent(int sc)
	{
		position.x = 400;
		position.y = 320;
		scale.x = scale.y = (float)sc;
	}

	TransformComponent(int x, int y, int w, int h, float scaleX, float scaleY, int origin)
	{
		position.x = (float)x;
		position.y = (float)y;

		dimensions.x = (float)w;
		dimensions.y = (float)h;

		scale.x = (float)scaleX;
		scale.y = (float)scaleY;

		speed = 3;

		SetOrigin(origin);
	}

	TransformComponent(Vector2D pos, Vector2D dim, Vector2D sca, int origin)
	{
		position = pos;
		dimensions = dim;
		scale = sca;

		speed = 3;

		SetOrigin(origin);
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

	void XYFromDegrees(float deg)
	{
		//Assume radius is 1
		velocity = Vector2D(sin(deg), -cos(deg));
	}

	void SetOrigin(int originType)
	{
		Vector2D offset = CalculateOffset(originType);
		position.x = position.x - ((dimensions.x * scale.x) * offset.x);
		position.y = position.y - ((dimensions.y * scale.y) * offset.y);
	}

	Vector2D CalculateOffset(int originType)
	{
		Vector2D offsetScalar;
		offsetScalar.Zero();
		switch (originType) {
		case 0:
			//Do nothing. Spawns top left by default.
			break;
		case 1:
			offsetScalar.x = 0.5;
			break;
		case 2:
			offsetScalar.x = 1.0;
			break;
		case 3:
			offsetScalar.y = 0.5;
			break;
		case 4:
			offsetScalar.x = 0.5;
			offsetScalar.y = 0.5;
			break;
		case 5:
			offsetScalar.x = 1.0;
			offsetScalar.y = 0.5;
			break;
		case 6:
			offsetScalar.y = 1.0;
			break;
		case 7:
			offsetScalar.x = 0.5;
			offsetScalar.y = 1.0;
			break;
		case 8:
			offsetScalar.x = 1.0;
			offsetScalar.y = 1.0;
			break;
		default:
			break;
		}

		return offsetScalar;
	}
};

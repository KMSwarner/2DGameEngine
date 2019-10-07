#pragma once
#include <SDL.h>
#include "Vector2D.h"

typedef std::pair<double, double> TriPoint;
class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool AABB(const ColliderComponent& a, const ColliderComponent& b);

#pragma region Tri Collision
	//static bool Triangle(const SDL_Rect& rectA, const SDL_Rect& rectB);
	////static Vector2D GetDifference(const TransformComponent &a, const TransformComponent &b);

	//static inline double Det2D(TriPoint &p1, TriPoint &p2, TriPoint &p3)
	//{
	//	return +p1.first*(p2.second - p3.second) +
	//			p2.first*(p3.second - p1.second) +
	//			p3.first*(p1.second - p2.second) ;
	//}

	//static void CheckTriWinding(TriPoint &p1, TriPoint &p2, TriPoint &p3, bool allowReversed);
	//static bool TriTri2D(TriPoint *t1, TriPoint *t2);
#pragma endregion
};
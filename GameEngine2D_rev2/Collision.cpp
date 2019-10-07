#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b)
{
	if (a.x + a.w >= b.x &&
		b.x + b.w >= a.x &&
		a.y + a.h >= b.y &&
		b.y + b.h >= a.y
		)
		return true;

	return false;
}

bool Collision::AABB(const ColliderComponent& a, const ColliderComponent& b)
{
	if (&a != &b && AABB(a.collider, b.collider))
	{
		//std::cout << a.tag << "hit " << b.tag << std::endl;
		return true;
	}
	return false;
}

#pragma region Tri Collision
//bool Collision::Triangle(const SDL_Rect& rectA, const SDL_Rect& rectB)
//{
//	//Separate the rects into two tris each
//	TriPoint tA1[] = {	TriPoint(rectA.x, rectA.y),
//						TriPoint(rectA.x + rectA.w, rectA.y),
//						TriPoint(rectA.x + rectA.w, rectA.y + rectA.h)
//					 }; //Represents the top right triangle of rectA
//
//	TriPoint tA2[] = {	TriPoint(rectA.x, rectA.y),
//						TriPoint(rectA.x, rectA.y + rectA.h),
//						TriPoint(rectA.x + rectA.w, rectA.y + rectA.h)
//					 }; //Represents the low left triangle of rectA
//
//	TriPoint tB1[] = {	TriPoint(rectB.x, rectB.y),
//						TriPoint(rectB.x + rectB.w, rectB.y),
//						TriPoint(rectB.x + rectB.w, rectB.y + rectB.h)
//					 }; //Represents the top right triangle of rectB
//
//	TriPoint tB2[] = {	TriPoint(rectB.x, rectB.y),
//						TriPoint(rectB.x, rectB.y + rectB.h),
//						TriPoint(rectB.x + rectB.w, rectB.y + rectB.h)
//					 }; //Represents the low left triangle of rectB
//
//	return (Collision::TriTri2D(tA1, tB1) || TriTri2D(tA1, tB2) || TriTri2D(tA2, tB1) || TriTri2D(tA2, tB2));
//}
//
//void Collision::CheckTriWinding(TriPoint & p1, TriPoint & p2, TriPoint & p3, bool allowReversed)
//{
//	int detTri = Det2D(p1, p2, p3);
//
//	if (detTri < 0)
//	{
//		if (allowReversed)
//		{
//			TriPoint a = p3;
//			p3 = p2;
//			p2 = a;
//		}
//
//		else throw std::runtime_error("triangle has wrong winding direction");
//	}
//}
//
//bool BoundaryCollideChk(TriPoint &p1, TriPoint &p2, TriPoint &p3, double eps)
//{
//	return Collision::Det2D(p1, p2, p3) < eps;
//}
//
//bool BoundaryDoesntCollideChk(TriPoint &p1, TriPoint &p2, TriPoint &p3, double eps)
//{
//	return Collision::Det2D(p1, p2, p3) <= eps;
//}
//
//bool Collision::TriTri2D(TriPoint * t1, TriPoint * t2)
//{
//	double eps;
//	bool allowReversed;
//	bool onBoundary;
//
//	bool(*chkEdge)(TriPoint&, TriPoint&, TriPoint&, double) = NULL;
//
//	eps = 0.0;
//	allowReversed = false;
//	onBoundary = true;
//
//	Collision::CheckTriWinding(t1[0], t1[1], t1[2], allowReversed);
//	Collision::CheckTriWinding(t2[0], t2[1], t2[2], allowReversed);
//
//	if (onBoundary) //Points on this boundary are considered as colliding
//		chkEdge = BoundaryCollideChk;
//	else
//		chkEdge = BoundaryDoesntCollideChk;
//
//	for (int i = 0; i < 3; i++)
//	{
//		int j = (i + 1) % 3;
//
//		if (
//			chkEdge(t1[i], t1[j], t2[0], eps) &&
//			chkEdge(t1[i], t1[j], t2[1], eps) &&
//			chkEdge(t1[i], t1[j], t2[2], eps))
//			return false;
//	}
//
//	for (int i = 0; i < 3; i++)
//	{
//		int j = (i + 1) % 3;
//
//		if (chkEdge(t2[i], t2[j], t1[0], eps) &&
//			chkEdge(t2[i], t2[j], t1[1], eps) &&
//			chkEdge(t2[i], t2[j], t1[2], eps))
//			return false;
//	}
//
//	//Otherwise, the triangles collide
//	return true;
//}
#pragma endregion

//Determining if rotated object has overlapped:
//
//Imagine any regular polygon as collection of tris. Squares and rects comprise
//two like, but inverse tris. Process must determine the locations of each
//vertex, then determine if the edges/paths between them overlap the edges/paths
//of another object's set of tris.
//Filter by tag so that a player's tris are never considered to be overlapping,
//and likewise for static map elements. Only detect for physical object
//interactions between player, wall, and others (npc, enemies, etc.).

//Vector2D Collision::GetDifference(const TransformComponent &a, const TransformComponent &b)
//{
//	int x, y;
//
//	x = a.position.x +a.
//	
//	return Vector2D();
//}

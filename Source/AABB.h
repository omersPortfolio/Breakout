#pragma once

#include "vec2.h"
#include "LineSegment.h"

// Axis-Aligned Bounding Box struct
struct AABB
{
	AABB(const vec2& m_Lower, const vec2& m_Upper);
	AABB();
	~AABB() = default;

	vec2 Center() const;
	vec2 GetDimensions() const;
	vec2 GetHalfDimensions() const;

	bool DoesIntersectWithCircle(const LineSegment& circlePath, float circleRadius, vec2& intersectionPoint, vec2& collisionNormal);

	bool Intersects(const AABB& other) const;
	bool Contains(const AABB& other) const;

	vec2 m_Lower;
	vec2 m_Upper;
};
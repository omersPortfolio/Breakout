#pragma once

#include "vec2.h"

struct LineSegment
{
	LineSegment(const vec2& start, const vec2& end);
	LineSegment(float startX, float startY, float endX, float endY);
	LineSegment();

	float Length() const;
	vec2 Normal() const;
	vec2 GetMidpoint() const;
	vec2 GetDelta() const;

	bool DoesIntersect(const LineSegment& lineSegment, vec2& intersectionPoint);

	vec2 m_Start;
	vec2 m_End;
};
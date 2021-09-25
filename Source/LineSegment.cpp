#include "GamePCH.h"

#include "LineSegment.h"


LineSegment::LineSegment(const vec2& start, const vec2& end) :
	m_Start(start),
	m_End(end)
{

}

LineSegment::LineSegment(float startX, float startY, float endX, float endY) :
	m_Start(startX, startY),
	m_End(endX, endY)
{

}

LineSegment::LineSegment() :
	m_Start(0.0f, 0.0f),
	m_End(0.0f, 0.0f)
{

}

float LineSegment::Length() const
{
	float distance = m_Start.Distance(m_End);
	return distance;
}

vec2 LineSegment::Normal() const
{
	vec2 normal = m_Start - m_End;
	return normal.PerpendicularCounterClockwise().GetNormalized();
}

vec2 LineSegment::GetMidpoint() const
{
	return vec2((m_Start + m_End) * 0.5f);
}

vec2 LineSegment::GetDelta() const
{
	return vec2(m_End.x - m_Start.x, m_End.y - m_Start.y);
}

bool LineSegment::DoesIntersect(const LineSegment& lineSegment, vec2& intersectionPoint)
{
	vec2 deltaA = GetDelta();
	vec2 deltaB = lineSegment.GetDelta();

	float s = (-deltaA.y * (m_Start.x - lineSegment.m_Start.x) + deltaA.x * (m_Start.y - lineSegment.m_Start.y)) / (-deltaB.x * deltaA.y + deltaA.x * deltaB.y);
	float t = (deltaB.x * (m_Start.y - lineSegment.m_Start.y) - deltaB.y * (m_Start.x - lineSegment.m_Start.x)) / (-deltaB.x * deltaA.y + deltaA.x * deltaB.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		//Collision detected
		intersectionPoint.x = m_Start.x + (t * deltaA.x);
		intersectionPoint.y = m_Start.y + (t * deltaA.y);
		return true;
	}

	//No collision
	return false;
}


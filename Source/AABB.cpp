#include "GamePCH.h"

#include "AABB.h"

AABB::AABB(const vec2& m_Lower, const vec2& m_Upper)
	: m_Lower(m_Lower),
	m_Upper(m_Upper)
{

}

AABB::AABB()
	: m_Lower(0.0f, 0.0f),
	m_Upper(1.0f, 1.0f)
{

}

vec2 AABB::Center() const
{
	return m_Lower + GetHalfDimensions();
}

vec2 AABB::GetDimensions() const
{
	return m_Upper - m_Lower;
}

vec2 AABB::GetHalfDimensions() const
{
	return GetDimensions() * 0.5f;
}

bool AABB::DoesIntersectWithCircle(const LineSegment& circlePath, float circleRadius, vec2& intersectionPoint, vec2& collisionNormal)
{
    // From: https://stackoverflow.com/questions/18704999/how-to-fix-circle-and-rectangle-overlap-in-collision-response/18790389#18790389
    // Converted to C++

	vec2 m_Start = circlePath.m_Start;
	vec2 m_End = circlePath.m_End;

    // T and B are flipped, this code assumes a top-left origin
    float L = m_Lower.x;
    float T = m_Lower.y;
    float R = m_Upper.x;
    float B = m_Upper.y;

    // If the bounding box around the start and end points (+radius on all sides)
    //    does not intersect with the rectangle, definitely not an intersection.
    if ((fmaxf(m_Start.x, m_End.x) + circleRadius < L) ||
        (fminf(m_Start.x, m_End.x) - circleRadius > R) ||
        (fmaxf(m_Start.y, m_End.y) + circleRadius < T) ||
        (fminf(m_Start.y, m_End.y) - circleRadius > B))
    {
        return false;
    }

    float dx = m_End.x - m_Start.x;
    float dy = m_End.y - m_Start.y;
    float invdx = (dx == 0.0f ? 0.0f : 1.0f / dx);
    float invdy = (dy == 0.0f ? 0.0f : 1.0f / dy);
    float cornerX = FLT_MAX;
    float cornerY = FLT_MAX;

    // Calculate intersection times with each side's plane.
    // Check each side's quadrant for single-side intersection.
    // Calculate Corner.

    /** Left Side **/
    // Does the circle go from the left side to the right side of the rectangle's left?
    if (m_Start.x - circleRadius < L && m_End.x + circleRadius > L)
    {
        float ltime = ((L - circleRadius) - m_Start.x) * invdx; 
        if (ltime >= 0.0f && ltime <= 1.0f)
        {
            float ly = dy * ltime + m_Start.y;
            // Does the collisions point lie on the left side?
            if (ly >= T && ly <= B)
            {
                intersectionPoint.Set(dx * ltime + m_Start.x, ly);
                collisionNormal.Set(-1, 0);
                return true;
            }
        }
        cornerX = L;
    }

    /** Right Side **/
    // Does the circle go from the right side to the left side of the rectangle's right?
    if (m_Start.x + circleRadius > R && m_End.x - circleRadius < R)
    {
        float rtime = (m_Start.x - (R + circleRadius)) * -invdx;
        if (rtime >= 0.0f && rtime <= 1.0f)
        {
            float ry = dy * rtime + m_Start.y;
            // Does the collisions point lie on the right side?
            if (ry >= T && ry <= B)
            {
                intersectionPoint.Set(dx * rtime + m_Start.x, ry);
                collisionNormal.Set(1, 0);
                return true;
            }
        }

        if (cornerX == FLT_MAX)
        {
            cornerX = R;
        }
        else
        {
            // Pick the nearer of the Left or Right corner to the start.
            // This is a problem if the circle is larger than the aabb width.
            float distToRCorner = fabs(R - m_Start.x);
            float distToLCorner = fabs(L - m_Start.x);

            if (distToRCorner < distToLCorner)
                cornerX = R;
            else
                cornerX = L;
        }
    }

    /** Top Side **/
    // Does the circle go from the top side to the bottom side of the rectangle's top?
    if (m_Start.y - circleRadius < T && m_End.y + circleRadius > T)
    {
        float ttime = ((T - circleRadius) - m_Start.y) * invdy;
        if (ttime >= 0.0f && ttime <= 1.0f)
        {
            float tx = dx * ttime + m_Start.x;
            // Does the collisions point lie on the top side?
            if (tx >= L && tx <= R)
            {
                intersectionPoint.Set(tx, dy * ttime + m_Start.y);
                collisionNormal.Set(0, -1);
                return true;
            }
        }
        cornerY = T;
    }

    /** Bottom Side **/
    // Does the circle go from the bottom side to the top side of the rectangle's bottom?
    if (m_Start.y + circleRadius > B && m_End.y - circleRadius < B)
    {
        float btime = (m_Start.y - (B + circleRadius)) * -invdy;
        if (btime >= 0.0f && btime <= 1.0f)
        {
            float bx = dx * btime + m_Start.x;
            // Does the collisions point lie on the bottom side?
            if (bx >= L && bx <= R)
            {
                intersectionPoint.Set(bx, dy * btime + m_Start.y);
                collisionNormal.Set(0, 1);
                return true;
            }
        }

        if (cornerY == FLT_MAX)
        {
            cornerY = B;
        }
        else
        {
            // Pick the nearer of the Top or Bottom corner to the start.
            // This is a problem if the circle is larger than the aabb height.
            float distToBCorner = fabs(B - m_Start.y);
            float distToTCorner = fabs(T - m_Start.y);

            if (distToBCorner < distToTCorner)
                cornerY = B;
            else
                cornerY = T;
        }
    }

    // No intersection at all!
    if (cornerX == FLT_MAX && cornerY == FLT_MAX)
    {
        return false;
    }

    // Account for the times where we don't pass over a side but we do hit it's corner.
    if (cornerX != FLT_MAX && cornerY == FLT_MAX)
    {
        cornerY = (dy > 0.0f ? B : T);
    }
    if (cornerY != FLT_MAX && cornerX == FLT_MAX)
    {
        cornerX = (dx > 0.0f ? R : L);
    }

    /* Solve the triangle between the start, corner, and intersection point.
    *
    *           +-----------T-----------+
    *           |                       |
    *          L|                       |R
    *           |                       |
    *           C-----------B-----------+
    *          / \
    *         /   \r     _.-E
    *        /     \ _.-'
    *       /    _.-I
    *      / _.-'
    *     S-'
    *
    * S = start of circle's path
    * E = end of circle's path
    * LTRB = sides of the rectangle
    * I = {ix, iY} = point at which the circle intersects with the rectangle
    * C = corner of intersection (and collision point)
    * C=>I (r) = {nx, ny} = radius and intersection normal
    * S=>C = cornerdist
    * S=>I = intersectionDistance
    * S=>E = lineLength
    * <S = innerAngle
    * <I = angle1
    * <C = angle2
    */

    double inverseRadius = 1.0 / circleRadius;
    double lineLength = sqrt(dx * dx + dy * dy);
    double cornerdx = cornerX - m_Start.x;
    double cornerdy = cornerY - m_Start.y;
    double cornerDistance = sqrt(cornerdx * cornerdx + cornerdy * cornerdy);
    double innerAngle = acos((cornerdx * dx + cornerdy * dy) / (lineLength * cornerDistance));

    // If the circle is too close, no intersection.
    if (cornerDistance < circleRadius)
    {
        return false;
    }

    // If inner angle is zero, it's going to hit the corner straight on.
    if (innerAngle == 0.0f)
    {
        float time = (float)((cornerDistance - circleRadius) / lineLength);

        // If time is outside the boundaries, return null. This algorithm can
        // return a negative time which indicates a previous intersection, and
        // can also return a time > 1.0f which can predict a corner intersection.
        if (time > 1.0f || time < 0.0f)
        {
            return false;
        }

        float ix = time * dx + m_Start.x;
        float iy = time * dy + m_Start.y;
        float nx = (float)(cornerdx / cornerDistance);
        float ny = (float)(cornerdy / cornerDistance);

        intersectionPoint.Set(ix, iy);
        collisionNormal.Set(nx, ny);
        return true;
    }

    double innerAngleSin = sin(innerAngle);
    double angle1Sin = innerAngleSin * cornerDistance * inverseRadius;

    // The angle is too large, there cannot be an intersection.
    if (abs(angle1Sin) > 1.0f)
    {
        return false;
    }

    double angle1 = M_PI - asin(angle1Sin);
    double angle2 = M_PI - innerAngle - angle1;
    double intersectionDistance = circleRadius * sin(angle2) / innerAngleSin;

    // Solve for time.
    float time = (float)(intersectionDistance / lineLength);

    // If time is outside the boundaries, return null. This algorithm can
    // return a negative time which indicates a previous intersection, and
    // can also return a time > 1.0f which can predict a corner intersection.
    if (time > 1.0f || time < 0.0f)
    {
        return false;
    }

    // Solve the intersection and normal.
    float ix = time * dx + m_Start.x;
    float iy = time * dy + m_Start.y;
    float nx = (float)((ix - cornerX) * inverseRadius);
    float ny = (float)((iy - cornerY) * inverseRadius);

    intersectionPoint.Set(ix, iy);
    collisionNormal.Set(nx, ny);
    return true;
}

bool AABB::Intersects(const AABB& other) const
{
	if (m_Upper.x < other.m_Lower.x)
		return false;

	if (m_Upper.y < other.m_Lower.y)
		return false;

	if (m_Lower.x > other.m_Upper.x)
		return false;

	if (m_Lower.y > other.m_Upper.y)
		return false;

	return true;
}

bool AABB::Contains(const AABB& other) const
{
	if (other.m_Lower.x >= m_Lower.x &&
		other.m_Upper.x <= m_Upper.x &&
		other.m_Lower.y >= m_Lower.y &&
		other.m_Upper.y <= m_Upper.y)
		return true;

	return false;
}

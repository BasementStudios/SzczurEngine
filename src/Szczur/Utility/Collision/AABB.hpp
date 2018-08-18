#pragma once

#include <glm/vec2.hpp>

namespace rat
{

class AABB
{
public:
	glm::vec2 centerPos;
	glm::vec2 size;

	AABB() : centerPos(0), size(0)
	{

	}

	AABB(const glm::vec2& centerPos, const glm::vec2& size)
		: centerPos(centerPos), size(size)
	{
	}

	glm::vec2 getMin() const
	{
		return centerPos - size;
	}

	glm::vec2 getMax() const
	{
		return centerPos + size;
	}

	glm::vec2 getSize() const
	{
		return size * 2.f;
	}

	bool isCollision()
	{
		auto min = getMin();
		auto max = getMax();

		return (min.x <= 0 & max.x >= 0 && min.y <= 0 && max.y >= 0);
	}

	void minkowskiDifference(const AABB& other, AABB& result)
	{
		auto topLeft = getMin() - other.getMax();
		auto fullSize = getSize() + other.getSize();

		result = AABB(topLeft + (fullSize / 2.f), fullSize / 2.f);
	}

	glm::vec2 closestPointOnBoundsToPoint(const glm::vec2& point)
	{
		glm::vec2 min = getMin();
		glm::vec2 max = getMax();

		float minDist = abs(point.x - min.x);
		glm::vec2 boundsPoint(min.x, point.y);
		if (abs(max.x - point.x) < minDist)
		{
			minDist = abs(max.x - point.x);
			boundsPoint = glm::vec2(max.x, point.y);
		}
		if (abs(max.y - point.y) < minDist)
		{
			minDist = abs(max.y - point.y);
			boundsPoint = glm::vec2(point.x, max.y);
		}
		if (abs(min.y - point.y) < minDist)
		{
			minDist = abs(min.y - point.y);
			boundsPoint = glm::vec2(point.x, min.y);
		}
		return boundsPoint;
	}

};

}

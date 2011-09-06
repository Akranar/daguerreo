#include "BasicPrimitiveTests.h"
#include "GeometricPrimitives.h"
#include "ExLib_BoundingVolume.h"
#include <algorithm>

float BasicPrimitiveTests::Clamp(float to_clamp, float min, float max)
{
	if (to_clamp < min) return min;
	if (to_clamp > max) return max;
	return to_clamp;
}

void BasicPrimitiveTests::Swap(float & x, float & y)
{
	float temp = y;
	y = x;
	x = temp;
}

float BasicPrimitiveTests::ScalarTripleProduct(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c)
{
	return a.dot(b.cross(c));
}

bool BasicPrimitiveTests::Between0And1(float arg)
{
	return arg >= 0.0f && arg <= 1.0f;
}

bool BasicPrimitiveTests::TestBounding(BoundingVolume * a, BoundingVolume * b)
{
	if (a->GetType() == BoundingVolume::BV_SPHERE)
	{
		switch (b->GetType())
		{
			case BoundingVolume::BV_SPHERE:
				return TestingSphereAgainstSphere(*((BoundingSphere *)b), *((BoundingSphere *)a));
				break;
			case BoundingVolume::BV_AABB:
				return TestingSphereAgainstAABB(*((BoundingSphere *)a), *((AABB *)b));
				break;
			case BoundingVolume::BV_OBB:
				return TestingSphereAgainstOBB(*((BoundingSphere *)a), *((OBB *)b));
				break;
			default:
				throw 1;
				break;
		}
	}
	else if (a->GetType() == BoundingVolume::BV_AABB)
	{
		switch (b->GetType())
		{
			case BoundingVolume::BV_SPHERE:
				return BasicPrimitiveTests::TestingSphereAgainstAABB(*((BoundingSphere *)b), *((AABB *)a));
				break;
			case BoundingVolume::BV_AABB:
				return BasicPrimitiveTests::TestingAABBAgainstAABB(*((AABB *)a), *((AABB *)b));
				break;
			case BoundingVolume::BV_OBB:
				return BasicPrimitiveTests::TestingAABBAgainstOBB(*((AABB *)a), *((OBB *)b));
				break;
			default:
				throw 1;
				break;
		}
	}
	else if (a->GetType() == BoundingVolume::BV_OBB)
	{
		switch (b->GetType())
		{
			case BoundingVolume::BV_SPHERE:
				return BasicPrimitiveTests::TestingSphereAgainstOBB(*((BoundingSphere *)b), *((OBB *)a));
				break;
			case BoundingVolume::BV_AABB:
				return BasicPrimitiveTests::TestingAABBAgainstOBB(*((AABB *)b), *((OBB *)a));
				break;
			case BoundingVolume::BV_OBB:
				return BasicPrimitiveTests::TestingOBBAgainstOBB(*((OBB *)a), *((OBB *)b));
				break;
			default:
				throw 1;
				break;
		}
	}
	throw 1;
}



#include "BoundCreation.h"
#include "ExLib_FloatDataAccessor.h"
#include "ExLib_Topology.h"

#include <float.h>

#include <iostream>

void BoundCreation::ExtremePointsAlongDirection(const Eigen::Vector3f & dir, const FloatDataAccessor & point_set, unsigned int & min_index, unsigned int & max_index)
{
	float min_projection = FLT_MAX, max_projection = -FLT_MAX;
	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		float projection = dir.dot(VEC3(point_set[i]));
		if (projection < min_projection)
		{
			min_projection = projection;
			min_index = i;
		}

		if (projection > max_projection)
		{
			max_projection = projection;
			max_index = i;
		}
	}
}

void BoundCreation::ExtremePointsAlongStandardAxes(const FloatDataAccessor & point_set, 
												   unsigned int & x_min_index, unsigned int & x_max_index,
												   unsigned int & y_min_index, unsigned int & y_max_index,
												   unsigned int & z_min_index, unsigned int & z_max_index
												   )
{
	x_min_index = 0;
	x_max_index = 0;
	y_min_index = 0;
	y_max_index = 0;
	z_min_index = 0;
	z_max_index = 0;

	for (unsigned int i = 1; i < point_set.GetCount(); ++i)
	{
		Eigen::Vector3f & point = VEC3(point_set[i]);
		if (point[0] < VEC3(point_set[x_min_index])[0]) x_min_index = i;
		if (point[0] > VEC3(point_set[x_max_index])[0]) x_max_index = i;
		if (point[1] < VEC3(point_set[y_min_index])[1]) y_min_index = i;
		if (point[1] > VEC3(point_set[y_max_index])[1]) y_max_index = i;
		if (point[2] < VEC3(point_set[z_min_index])[2]) z_min_index = i;
		if (point[2] > VEC3(point_set[z_max_index])[2]) z_max_index = i;
	}
}

void BoundCreation::CovarianceMatrix(const FloatDataAccessor & point_set, Eigen::Matrix3f & rtn_covariance)
{
	float oon = 1.0f / (float) point_set.GetCount();
	Eigen::Vector3f c = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	float e00, e11, e22, e01, e02, e12;

	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		c += VEC3(point_set[i]);
	}
	c *= oon;

	// Compute covariance elements
	e00 = e11 = e22 = e01 = e02 = e12 = 0.0f;
	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		Eigen::Vector3f p = VEC3(point_set[i]) - c;

		e00 += p[0] * p[0];
		e11 += p[1] * p[1];
		e22 += p[2] * p[2];
		e01 += p[0] * p[1];
		e02 += p[0] * p[2];
		e12 += p[1] * p[2];
	}

	rtn_covariance.coeffRef(0, 0) = e00 * oon;
	rtn_covariance.coeffRef(1, 1) = e11 * oon;
	rtn_covariance.coeffRef(2, 2) = e22 * oon;
	rtn_covariance.coeffRef(0, 1) = rtn_covariance.coeffRef(1, 0) = e01 * oon;
	rtn_covariance.coeffRef(0, 2) = rtn_covariance.coeffRef(2, 0) = e02 * oon;
	rtn_covariance.coeffRef(1, 2) = rtn_covariance.coeffRef(2, 1) = e12 * oon;
}

void GetMax(unsigned int start, unsigned int end, const float * data, unsigned int & rtn_max_index)
{
	rtn_max_index = 0;
	float value = data[0];
	for (unsigned int i = 1; i < end; ++i)
	{
		if (data[i] >= value)
		{
			value = data[i];
			rtn_max_index = i;
		}
	}
}
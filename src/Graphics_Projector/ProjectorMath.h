#ifndef _PROJECTOR_MATH_H_
#define _PROJECTOR_MATH_H_

#include <cmath>

template<bool column_major>
class ProjectorMath
{
};

template<>
class ProjectorMath<true>
{
public:
	static inline void Normalize(float vect[3]);
	static inline void MatrixMultiply4x4(float * destination, const float * source_a, const float * source_b);
	static void ViewMatrix(const float * position, const float * direction, const float * up, float * out_view_matrix);
	static inline void LookAtMatrix(const float * position, const float * target, const float * up, float * out_view_matrix);
	static void PerspectiveProjectionMatrix(float left, float right, float bottom, float top, float distance_to_near, float distance_to_far, float * out_projection_matrix);
	static void PerspectiveProjectionMatrix(float fov, float aspect_ratio, float distance_to_near, float distance_to_far, float * out_projection_matrix);
	static void OrthographicProjectionMatrix(float left, float right, float bottom, float top, float distance_to_near, float distance_to_far, float * out_projection_matrix);
};

#include "ProjectorMath_ColumnMajor.h"

template<>
class ProjectorMath<false>
{
public:
	static inline void Normalize(float vect[3]);
	static inline void MatrixMultiply4x4(float * destination, const float * source_a, const float * source_b);

	static inline void LookAtMatrix(const float * position, const float * target, const float * up, float * out_view_matrix);
	static void ViewMatrix(const float * position, const float * direction, const float * up, float * out_view_matrix);
	static void PerspectiveProjectionMatrix(float left, float right, float bottom, float top, float distance_to_near, float distance_to_far, float * out_projection_matrix);
	static void PerspectiveProjectionMatrix(float fov, float aspect_ratio, float distance_to_near, float distance_to_far, float * out_projection_matrix);
	static void OrthographicProjectionMatrix(float left, float right, float bottom, float top, float distance_to_near, float distance_to_far, float * out_projection_matrix);
};

#include "ProjectorMath_RowMajor.h"

#endif
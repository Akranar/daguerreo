

inline void ProjectorMath<false>::Normalize(float vect[3])
{
	float length_sq = vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2];
	if (length_sq == 0) return;
	float length = sqrt(length_sq);
	if (length - 1 < 1e-20) return;
	vect[0] /= length;
	vect[1] /= length;
	vect[2] /= length;
}

inline void ProjectorMath<false>::MatrixMultiply4x4(float * destination, const float * source_a, const float * source_b)
{
	destination[0] = (source_a[0] * source_b[0]) + (source_a[4] * source_b[1]) + (source_a[8] * source_b[2]) + (source_a[12] * source_b[3]);
	destination[1] = (source_a[1] * source_b[0]) + (source_a[5] * source_b[1]) + (source_a[9] * source_b[2]) + (source_a[13] * source_b[3]);
	destination[2] = (source_a[2] * source_b[0]) + (source_a[6] * source_b[1]) + (source_a[10] * source_b[2]) + (source_a[14] * source_b[3]);
	destination[3] = (source_a[3] * source_b[0]) + (source_a[7] * source_b[1]) + (source_a[11] * source_b[2]) + (source_a[15] * source_b[3]);
	destination[4] = (source_a[0] * source_b[4]) + (source_a[4] * source_b[5]) + (source_a[8] * source_b[6]) + (source_a[12] * source_b[7]);
	destination[5] = (source_a[1] * source_b[4]) + (source_a[5] * source_b[5]) + (source_a[9] * source_b[6]) + (source_a[13] * source_b[7]);
	destination[6] = (source_a[2] * source_b[4]) + (source_a[6] * source_b[5]) + (source_a[10] * source_b[6]) + (source_a[14] * source_b[7]);
	destination[7] = (source_a[3] * source_b[4]) + (source_a[7] * source_b[5]) + (source_a[11] * source_b[6]) + (source_a[15] * source_b[7]);
	destination[8] = (source_a[0] * source_b[8]) + (source_a[4] * source_b[9]) + (source_a[8] * source_b[10]) + (source_a[12] * source_b[11]);
	destination[9] = (source_a[1] * source_b[8]) + (source_a[5] * source_b[9]) + (source_a[9] * source_b[10]) + (source_a[13] * source_b[11]);
	destination[10] = (source_a[2] * source_b[8]) + (source_a[6] * source_b[9]) + (source_a[10] * source_b[10]) + (source_a[14] * source_b[11]);
	destination[11] = (source_a[3] * source_b[8]) + (source_a[7] * source_b[9]) + (source_a[11] * source_b[10]) + (source_a[15] * source_b[11]);
	destination[12] = (source_a[0] * source_b[12]) + (source_a[4] * source_b[13]) + (source_a[8] * source_b[14]) + (source_a[12] * source_b[15]);
	destination[13] = (source_a[1] * source_b[12]) + (source_a[5] * source_b[13]) + (source_a[9] * source_b[14]) + (source_a[13] * source_b[15]);
	destination[14] = (source_a[2] * source_b[12]) + (source_a[6] * source_b[13]) + (source_a[10] * source_b[14]) + (source_a[14] * source_b[15]);
	destination[15] = (source_a[3] * source_b[12]) + (source_a[7] * source_b[13]) + (source_a[11] * source_b[14]) + (source_a[15] * source_b[15]);
}

inline void ProjectorMath<false>::ViewMatrix(const float * position, const float * direction, const float * up, float * out_view_matrix)
{
	/*
		"right_direction" = cross(direction, up)	(OpenGL View Space is Right-handed)

		"direction" is in the negative direction:
		Camera Space Basis:
			-> Q = [R, U, -D]
				-> where R = "right_direction", = +X
				-> U = "up", = +Y
				-> D = -"direction" = -Z

		Rotation Matrix from World to Camera Space Basis = Q = Transpose(Q)
	*/
	float right_direction[3];
	right_direction[0] = direction[1] * up[2] - direction[2] * up[1];
	right_direction[1] = direction[2] * up[0] - direction[0] * up[2];
	right_direction[2] = direction[0] * up[1] - direction[1] * up[0];

	Normalize(right_direction);

	out_view_matrix[0] = right_direction[0];
	out_view_matrix[1] = right_direction[1];
	out_view_matrix[2] = right_direction[2];
	out_view_matrix[3] = - right_direction[0] * position[0] - right_direction[1] * position[1] - right_direction[2] * position[2];

	out_view_matrix[4] = up[0];
	out_view_matrix[5] = up[1];
	out_view_matrix[6] = up[2];
	out_view_matrix[7] = - up[0] * position[0] - up[1] * position[1] - up[2] * position[2];

	out_view_matrix[8] = -direction[0];
	out_view_matrix[9] = -direction[1];
	out_view_matrix[10] = -direction[2];
	out_view_matrix[11] = + direction[0] * position[0] + direction[1] * position[1] + direction[2] * position[2];

	out_view_matrix[12] = 0;
	out_view_matrix[13] = 0;
	out_view_matrix[14] = 0;
	out_view_matrix[15] = 1;
}

inline void ProjectorMath<false>::LookAtMatrix(const float * position, const float * target, const float * up, float * out_view_matrix)
{
	float direction[3];
	direction[0] = target[0] - position[0];
	direction[1] = target[1] - position[1];
	direction[2] = target[2] - position[2];

	float up_direction[3];
	up_direction[0] = up[0];
	up_direction[1] = up[1];
	up_direction[2] = up[2];

	Normalize(direction);
	Normalize(up_direction);

	ViewMatrix(position, direction, up_direction, out_view_matrix);
}

inline void ProjectorMath<false>::PerspectiveProjectionMatrix(float left, float right, float bottom, float top, float distance_to_near, float distance_to_far, float * out_projection_matrix)
{
	out_projection_matrix[0] = (2 * distance_to_near) / (right - left);
	out_projection_matrix[4] = 0;
	out_projection_matrix[8] = 0;
	out_projection_matrix[12] = 0;

	out_projection_matrix[1] = 0;
	out_projection_matrix[5] = (2 * distance_to_near) / (top - bottom);
	out_projection_matrix[9] = 0;
	out_projection_matrix[13] = 0;

	out_projection_matrix[2] = (right + left) / (right - left);
	out_projection_matrix[6] = (top + bottom) / (top - bottom);
	out_projection_matrix[10] = -(distance_to_far + distance_to_near) / (distance_to_far - distance_to_near);
	out_projection_matrix[14] = -1;

	out_projection_matrix[3] = 0;
	out_projection_matrix[7] = 0;
	out_projection_matrix[11] = (- 2 * distance_to_near * distance_to_far) / (distance_to_far - distance_to_near);
	out_projection_matrix[15] = 0;
}

inline void ProjectorMath<false>::PerspectiveProjectionMatrix(float fov, float aspect_ratio, float distance_to_near, float distance_to_far, float * out_projection_matrix)
{
	float top = float(tan(fov * 3.1415926536 / 360) * distance_to_near);
	float bottom = -top;
	float right = aspect_ratio * top;
	float left = -right;

	out_projection_matrix[0] = (2 * distance_to_near) / (right - left);
	out_projection_matrix[4] = 0;
	out_projection_matrix[8] = 0;
	out_projection_matrix[12] = 0;

	out_projection_matrix[1] = 0;
	out_projection_matrix[5] = (2 * distance_to_near) / (top - bottom);
	out_projection_matrix[9] = 0;
	out_projection_matrix[13] = 0;

	out_projection_matrix[2] = (right + left) / (right - left);
	out_projection_matrix[6] = (top + bottom) / (top - bottom);
	out_projection_matrix[10] = -(distance_to_far + distance_to_near) / (distance_to_far - distance_to_near);
	out_projection_matrix[14] = -1;

	out_projection_matrix[3] = 0;
	out_projection_matrix[7] = 0;
	out_projection_matrix[11] = (- 2 * distance_to_near * distance_to_far) / (distance_to_far - distance_to_near);
	out_projection_matrix[15] = 0;
}

inline void ProjectorMath<false>::OrthographicProjectionMatrix(float left, float right, float bottom, float top, float distance_to_near, float distance_to_far, float * out_projection_matrix)
{
	out_projection_matrix[0] = 2 / (right - left);
	out_projection_matrix[4] = 0;
	out_projection_matrix[8] = 0;
	out_projection_matrix[12] = 0;

	out_projection_matrix[1] = 0;
	out_projection_matrix[5] = 2 / (top - bottom);
	out_projection_matrix[9] = 0;
	out_projection_matrix[13] = 0;

	out_projection_matrix[2] = 0;
	out_projection_matrix[6] = 0;
	out_projection_matrix[10] = -2 / (distance_to_far - distance_to_near);
	out_projection_matrix[14] = 0;

	out_projection_matrix[3] = -(right + left) / (right - left);
	out_projection_matrix[7] = -(top + bottom) / (top - bottom);
	out_projection_matrix[11] = - (distance_to_far + distance_to_near) / (distance_to_far - distance_to_near);
	out_projection_matrix[15] = 1;
}


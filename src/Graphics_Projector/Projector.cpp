#include "Projector.h"
#include "ProjectorMath.h"

#include <iostream>

void Projector::ViewMatrix(float * out_view_matrix)
{
	ProjectorMath<false>::ViewMatrix(position, direction, up, out_view_matrix);
}
void Projector::PerspectiveMatrix(float * out_proj_matrix)
{
	ProjectorMath<false>::PerspectiveProjectionMatrix(left, right, bottom, top, near, far, out_proj_matrix);
}
void Projector::ViewProjMatrix(float * out_viewprojmatrix)
{
	float view[16];
	float proj[16];
	ProjectorMath<false>::ViewMatrix(position, direction, up, view);
	ProjectorMath<false>::PerspectiveProjectionMatrix(left, right, bottom, top, near, far, proj);
	ProjectorMath<false>::MatrixMultiply4x4(out_viewprojmatrix, view, proj);
}

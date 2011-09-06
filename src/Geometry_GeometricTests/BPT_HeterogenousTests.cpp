#include "BasicPrimitiveTests.h"

#include "ExLib_BoundingVolume.h"

bool BasicPrimitiveTests::TestingAABBAgainstOBB(const AABB & aabb, const OBB & obb)
{
	/*
		Separating Axis Test with 15 Axes:
			-> Three orientation axes of A (AABB)
			-> Three orientation axes of B (OBB)
			-> Nine from cross-products of A and B's orientation axes.
	*/

	float EPSILON = (float) 1e-20;

	/*
		Translation vector AB, from A's center to B's center.
			-> Express AB with respect to A's coordinate axes.
				-> AB_AFrame = Transpose([A0, A1, A2]) * T_StandardFrame
	*/
	Eigen::Vector3f AB_AFrame = obb.GetCenter() - aabb.GetCenter();

	
	/*
		Construct Matrix that expresses B's axes in A's frame.
			-> [B0_AFrame, B1_AFrame, B2_AFrame] = Transpose([A0, A1, A2]) * [B0, B1, B2]

			Will Also Have A's axes Expressed in B's frame:
				-> [A0_BFrame, A1_BFrame, A2_BFrame] = Transpose([B0_AFrame, B1_AFrame, B2_AFrame])

			Also Construct Absolute Value of previous matrix.
				-> Epsilon is added to counteract robustness problem
					-> When two edges are parallel and cross product is near zero.

		NOTE: Actual construction of matrices are interleaved with tests of A's axes to avoid unnecessary computation.
			-> Early out after finding one separating axis.
	*/
	Eigen::Matrix3f B_axes_AFrame, abs_B_axes_AFrame;
	/*
		Test A's Axes
			RadiusA (along Ai) = the (half)extent of current axis, Ai
			RadiusB (along Ai) = Length of Projection of B's Extent Vector onto Ai_BFrame
				-> dot(B_ExtentVector, Ai_BFrame)
				-> Use B's Extent Vector because it is the longest vector in the positive quadrant of OBB B.
					-> Possible due to having abs_B_axes_AFrame
						-> Which limits axes to positive quadrant.

			Test (Done in A's Frame):
				AbsoluteValue(    Length(AB) (along Ai)    ) > Radius_A + Radius_B
					-> abs(    AB_AFrame[i]    ) > Radius_A + Radius_B
	*/
	float radius_a, radius_b;
	for (int i = 0; i < 3; ++i)
	{
		/*
			Interleaved calculations of B_axes_AFrame and abs_B_axes_AFrame. {
		*/
		for (unsigned int j = 0; j < 3; ++j)
		{
			B_axes_AFrame.coeffRef(i, j) = obb.GetAxis(j)[i];
			abs_B_axes_AFrame.coeffRef(i, j) = abs(B_axes_AFrame.coeff(i, j)) + EPSILON;
		}
		/*
			Interleaved calculations of B_axes_AFrame and abs_B_axes_AFrame. }
			Test A's Axes {
		*/
		radius_a = aabb.GetExtent(i);
		radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(i, 0) + obb.GetExtent(1) * abs_B_axes_AFrame.coeff(i, 1) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(i, 2);
		if (abs(AB_AFrame[i]) > radius_a + radius_b) return false;
		/*
			Test A's Axes }
		*/
	}

	/*
		Test B's Axes
			RadiusA (along Bi) = Length of Projection of B's Extent Vector onto Bi_AFrame
				-> dot(A_ExtentVector, Bi_AFrame)
				-> Use A's Extent Vector because it is the longest vector in the positive quadrant of OBB A.
					-> Possible due to having abs_B_axes_AFrame
						-> Which limits axes to positive quadrant.
			RadiusB (along Bi) = the (half)extent of current axis, Bi

			Test (Done in A's Frame):
				AbsoluteValue(    Length(AB) (along Bi_AFrame)    ) > Radius_A + Radius_B
					-> abs(    dot(AB_AFrame, Bi_AFrame)    ) > Radius_A + Radius_B
	*/
	for (int i = 0; i < 3; ++i)
	{
		radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(0, i) + aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(1, i) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(2, i);
		radius_b = obb.GetExtent(i);
		if (abs(AB_AFrame[0] * B_axes_AFrame.coeff(0, i) + 
			AB_AFrame[1] * B_axes_AFrame.coeff(1, i) + 
			AB_AFrame[2] * B_axes_AFrame.coeff(2, i)) > radius_a + radius_b) return false;
	}

	
	/*
		Cross-Products simplified since we are in either A or B's Frame.
			A0 = <1, 0, 0>, 
			A1 = <0, 1, 0>, 
			and A2 = <0, 0, 1>

		A0xBi (A_Frame) = <0, -Bi.z, Bi.y>
		A1xBi (A_Frame) = <Bi.z, 0, -Bi.x>
		A2xBi (A_Frame) = <-Bi.y, -Bi.x, 0>

		B0xAi (B_Frame) = <0, -Ai.z, Ai.y>
		B1xAi (B_Frame) = <Ai.z, 0, -Ai.x>
		B2xAi (B_Frame) = <-Ai.y, -Ai.x, 0>

		Example with A0xB0:

			RadiusA:
				-> dot(A.ExtentVector, abs(    A0xBi_AFrame    ))
				-> A.YExtent * abs(B0.z) + A.ZExtent * abs(B0.y)

			RadiusB:
				-> dot(B.ExtentVector, abs(    B0xAi_BFrame    ))
				-> B.YExtent * abs(A0.z) + B.ZExtent * abs(A0.y)

			Test:
				Length(AB) (along A0xB0 and in AFrame) > RadiusA + RadiusB
					-> abs(    dot(AB_AFrame, A0xB0_AFrame)    ) > RadiusA + RadiusB
						-> abs(    AB.z * B0.y - AB.y * B0.z    ) > RadiusA + RadiusB
	*/
	//A0 x B0
	radius_a = aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(2, 0) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(1, 0);
	radius_b = obb.GetExtent(1) * abs_B_axes_AFrame.coeff(0, 2) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(0, 1);
	if (abs(AB_AFrame[2] * B_axes_AFrame.coeff(1, 0) - AB_AFrame[1] * B_axes_AFrame.coeff(2, 0)) > radius_a + radius_b) return false;
	//A0 x B1
	radius_a = aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(2, 1) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(1, 1);
	radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(0, 2) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(0, 0);
	if (abs(AB_AFrame[2] * B_axes_AFrame.coeff(1, 1) - AB_AFrame[1] * B_axes_AFrame.coeff(2, 1)) > radius_a + radius_b) return false;
	//A0 x B2
	radius_a = aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(2, 2) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(1, 2);
	radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(0, 1) + obb.GetExtent(1) * abs_B_axes_AFrame.coeff(0, 0);
	if (abs(AB_AFrame[2] * B_axes_AFrame.coeff(1, 2) - AB_AFrame[1] * B_axes_AFrame.coeff(2, 2)) > radius_a + radius_b) return false;

	//A1 x B0
	radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(2, 0) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(0, 0);
	radius_b = obb.GetExtent(1) * abs_B_axes_AFrame.coeff(1, 2) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(1, 1);
	if (abs(AB_AFrame[0] * B_axes_AFrame.coeff(2, 0) - AB_AFrame[2] * B_axes_AFrame.coeff(0, 0)) > radius_a + radius_b) return false;
	//A1 x B1
	radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(2, 1) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(0, 1);
	radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(1, 2) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(1, 0);
	if (abs(AB_AFrame[0] * B_axes_AFrame.coeff(2, 1) - AB_AFrame[2] * B_axes_AFrame.coeff(0, 1)) > radius_a + radius_b) return false;
	//A1 x B2
	radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(2, 2) + aabb.GetExtent(2) * abs_B_axes_AFrame.coeff(0, 2);
	radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(1, 1) + obb.GetExtent(1) * abs_B_axes_AFrame.coeff(1, 0);
	if (abs(AB_AFrame[0] * B_axes_AFrame.coeff(2, 2) - AB_AFrame[2] * B_axes_AFrame.coeff(0, 2)) > radius_a + radius_b) return false;

	//A2 x B0
	radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(1, 0) + aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(0, 0);
	radius_b = obb.GetExtent(1) * abs_B_axes_AFrame.coeff(2, 2) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(2, 1);
	if (abs(AB_AFrame[1] * B_axes_AFrame.coeff(0, 0) - AB_AFrame[0] * B_axes_AFrame.coeff(1, 0)) > radius_a + radius_b) return false;
	//A2 x B1
	radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(1, 1) + aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(0, 1);
	radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(2, 2) + obb.GetExtent(2) * abs_B_axes_AFrame.coeff(2, 0);
	if (abs(AB_AFrame[1] * B_axes_AFrame.coeff(0, 1) - AB_AFrame[0] * B_axes_AFrame.coeff(1, 1)) > radius_a + radius_b) return false;
	//A2 x B2
	radius_a = aabb.GetExtent(0) * abs_B_axes_AFrame.coeff(1, 2) + aabb.GetExtent(1) * abs_B_axes_AFrame.coeff(0, 2);
	radius_b = obb.GetExtent(0) * abs_B_axes_AFrame.coeff(2, 1) + obb.GetExtent(1) * abs_B_axes_AFrame.coeff(2, 0);
	if (abs(AB_AFrame[1] * B_axes_AFrame.coeff(0, 2) - AB_AFrame[0] * B_axes_AFrame.coeff(1, 2)) > radius_a + radius_b) return false;


	return true;
}
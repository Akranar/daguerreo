#ifndef _MATH_EIGEN_LIB_H_
#define _MATH_EIGEN_LIB_H_

#define EIGEN_NO_DEBUG

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include "Eigen/StdVector"

#define VEC2(x) (*((Eigen::Vector2f *) x))
#define VEC3(x) (*((Eigen::Vector3f *) x))
#define VEC4(x) (*((Eigen::Vector4f *) x))
#define MAT3(x) (*((Eigen::Matrix3f *) x))
#define MAT4(x) (*((Eigen::Matrix4f *) x))
#define AFF3(x) (*((Eigen::Affine3f *) x))

#endif




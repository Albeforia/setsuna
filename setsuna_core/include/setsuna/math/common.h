#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <setsuna/math/vector.h>
#include <setsuna/math/matrix.h>
#include <setsuna/math/quaternion.h>
#include <setsuna/math/transform.h>

/** @file
@brief Header for common math utilities and includes
*/

namespace setsuna {

/**
@brief Convert degrees to radians
*/
constexpr float to_radians(float degrees) {
	return degrees * (M_PI / 180.0f);
}

/**
@brief Convert radians to degrees
*/
constexpr float to_degrees(float radians) {
	return radians * (180.0f / M_PI);
}

/**
@brief Raise each component of @p v to the power of @p e
*/
inline vector4 pow(const vector4& v, float e) {
	return vector4(DirectX::XMVectorPow(v, vector4(e)));
}

/**
@brief Get a perspective projection matrix

@p fovy is in radians.
*/
inline matrix4 perspective(float fovy, float aspect, float near, float far) {
#ifdef _SETSUNA_LEFT_HANDED
	return matrix4(DirectX::
	                 XMMatrixPerspectiveFovLH(fovy, aspect, near, far));
#else
	return matrix4(DirectX::
	                 XMMatrixPerspectiveFovRH(fovy, aspect, near, far));
#endif  // _SETSUNA_LEFT_HANDED
}

/**
@brief Get a orthographic projection matrix
*/
inline matrix4 orthographic(float width, float height, float near, float far) {
#ifdef _SETSUNA_LEFT_HANDED
	return matrix4(DirectX::
	                 XMMatrixOrthographicLH(width, height, near, far));
#else
	return matrix4(DirectX::
	                 XMMatrixOrthographicRH(width, height, near, far));
#endif  // _SETSUNA_LEFT_HANDED
}

}  // namespace setsuna

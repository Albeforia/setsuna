#pragma once

#include <setsuna/math/vector.h>
#include <setsuna/math/matrix.h>
#include <setsuna/math/quaternion.h>

/** @file
@brief Header for @ref setsuna::transform
*/

namespace setsuna {

/**
@brief Affine transformation including translation, rotation and scaling

The rotation is stored as a quaternion which is not as intuitive as Euler angles.
Instead of directly modifying the quaternion, you could rotate a transform
@p angle degrees around some @p axis like this:

@code{.cpp}
quaternion q(axis, to_radians(angle));
trsfm.rotation = q * trsfm.rotation;
@endcode
*/
struct transform {

	/**
	@brief Default constructor
	*/
	transform() :
	    rotation(0.0f, 0.0f, 0.0f, 1.0f),  // x, y, z, w
	    scale(1.0f, 1.0f, 1.0f),
	    translation(0.0f, 0.0f, 0.0f) {}

	/**
	@brief Convert to an affine transformation matrix
	*/
	explicit operator matrix4() const {
		return matrix4(DirectX::XMMatrixAffineTransformation(
		  vector3(scale),
		  vector4(0, 0, 0), quaternion(rotation),
		  vector3(translation)));
	}

	float3 translation; /**< @brief Translation */
	float3 scale;       /**< @brief Scale */
	float4 rotation;    /**< @brief Rotation represented by a quaternion */
};

}  // namespace setsuna

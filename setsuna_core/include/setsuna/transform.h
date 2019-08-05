#pragma once

#include <DirectXMath.h>

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
// if 'axis' is not normalized, use XMQuaternionRotationAxis() instead
auto q = XMQuaternionRotationNormal(axis, XMConvertToRadians(angle));
XMStoreFloat4(
	&(trsfm.rotation),
	XMQuaternionMultiply(XMLoadFloat4(&(trsfm.rotation)), q)
);
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
	explicit operator DirectX::XMMATRIX() const;

	// stored as XMFLOAT* to avoid explicit alignment
	DirectX::XMFLOAT3 translation; /**< @brief Translation */
	DirectX::XMFLOAT3 scale;       /**< @brief Scale */
	DirectX::XMFLOAT4 rotation;    /**< @brief Rotation represented by a quaternion */
};

}  // namespace setsuna

#pragma once

#include <DirectXMath.h>

/** @file
@brief Header for @ref setsuna::plane
*/

namespace setsuna {

/**
@brief Mathematical plane
*/
struct plane {

	float x; /**< @brief The x component of the normal */
	float y; /**< @brief The y component of the normal */
	float z; /**< @brief The z component of the normal */
	float d; /**< @brief The signed distance from origin to the plane */

	/**
	@brief Default constructor
	*/
	plane() :
	    x{1.0f}, y{0.0f}, z{0.0f}, d{0.0f} {}

	/**
	@brief Construct from four coefficients
	*/
	plane(float x, float y, float z, float d) {
		auto np = DirectX::XMPlaneNormalize(DirectX::XMVectorSet(x, y, z, d));
		this->x = DirectX::XMVectorGetX(np);
		this->y = DirectX::XMVectorGetY(np);
		this->z = DirectX::XMVectorGetZ(np);
		this->d = DirectX::XMVectorGetW(np);
	}

	/**
	@brief Evaluate plane equation for point @p v

	@f[
	y = (x,y,z) \cdot v + d
	@f]

	@a y is the signed distance from @p v to the plane;
	@p v is on the positive side of the plane if @a y is greater than zero.
	*/
	float operator()(const DirectX::XMVECTOR& v) const {
		return DirectX::XMVectorGetX(
		  DirectX::XMPlaneDot(DirectX::XMVectorSet(x, y, z, d), v));
	}
};

}  // namespace setsuna

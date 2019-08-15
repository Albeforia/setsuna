#pragma once

#include <setsuna/math/vector.h>

/** @file
@brief Header for @ref setsuna::plane
*/

namespace setsuna {

/**
@brief Mathematical plane
*/
struct plane {

	/**
	@brief Four coefficients of the plane
	*/
	float4 coeffs;

	/**
	@brief Default constructor
	*/
	plane() :
	    coeffs(0, 0, 0, 0) {}

	/**
	@brief Construct from four coefficients
	*/
	plane(float x, float y, float z, float d) :
	    coeffs(vector4(DirectX::XMPlaneNormalize(DirectX::XMVectorSet(x, y, z, d)))) {
	}

	/**
	@brief Evaluate plane equation for point @p v

	@f[
	y = (x,y,z) \cdot v + w
	@f]

	@a y is the signed distance from @p v to the plane;
	@p v is on the positive side of the plane if @a y is greater than zero.
	*/
	float operator()(const vector3& v) const {
		return DirectX::XMVectorGetX(
		  DirectX::XMPlaneDot(vector4(coeffs), vector4(v, 1.0f)));
	}
};

}  // namespace setsuna

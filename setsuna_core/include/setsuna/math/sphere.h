#pragma once

#include <setsuna/math/vector.h>

/** @file
@brief Header for @ref setsuna::sphere
*/

namespace setsuna {

/**
@brief Mathematical sphere
*/
struct sphere {

	float3 center; /**< @brief Center of the sphere */
	float radius;  /**< @brief Radius of the sphere */

	/**
	@brief Default constructor, initialize to an invalid sphere
	*/
	sphere() :
	    center(0.0f, 0.0f, 0.0f), radius{0.0f} {}
};

}  // namespace setsuna

#pragma once

#include <glm/glm.hpp>

/** @file
@brief Header for @ref setsuna::sphere
*/

namespace setsuna {

/**
@brief Abstract sphere
*/
struct sphere {

	glm::vec3 center; /**< @brief Center of the sphere */
	float radius;     /**< @brief Radius of the sphere */

	/**
	@brief Default constructor, initialize to an invalid sphere
	*/
	sphere() :
	    radius{0.0f} {}

	/**
	@brief Construct from center @p c and radius @p r
	*/
	sphere(const glm::vec3& c, float r) :
	    center(c), radius{r} {}
};

}  // namespace setsuna

#pragma once

#include <glm/glm.hpp>

/** @file
@brief Header for @ref setsuna::plane
*/

namespace setsuna {

/**
@brief Abstract plane
*/
struct plane {

	glm::vec3 normal; /**< @brief The normal of the plane */
	float d;          /**< @brief The signed distance from origin to the plane */

	/**
	@brief Default constructor

	Initialize @ref #normal to (1, 0, 0) and @ref #d to 0.
	*/
	plane() :
	    normal(1.0f, 0.0f, 0.0f), d{0.0f} {}

	/**
	@brief Construct using normal @p n and distance @p d
	*/
	plane(const glm::vec3& n, float d) :
	    normal(n), d{d} {
		auto length = glm::length(normal);
		normal /= length;
		d /= length;
	}

	/**
	@brief Evaluate plane equation for point @p v
	
	@f[
	y = normal \cdot v + d
	@f]

	@a y is the signed distance from @p v to the plane,
	@p v is on the positive side of the plane if @a y is greater than zero.
	*/
	float operator()(const glm::vec3& v) const {
		return glm::dot(normal, v) + d;
	}
};

}  // namespace setsuna

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
trsfm.rotation = glm::rotate(trsfm.rotation, glm::radians(angle), axis);
@endcode
*/
struct transform {

	/**
	@brief Default constructor
	*/
	transform() :
	    rotation(1.0f, 0.0f, 0.0f, 0.0f),  // w, x, y, z
	    scale(1.0f, 1.0f, 1.0f),
	    translation(0.0f, 0.0f, 0.0f) {}

	/**
	@brief Convert to an affine transformation matrix
	*/
	explicit operator glm::mat4() const {
		auto result = glm::mat4(1.0f);
		result = glm::translate(result, translation);
		result *= glm::mat4_cast(rotation);
		result = glm::scale(result, scale);
		return result;
	}

	glm::vec3 translation; /**< @brief Translation */
	glm::vec3 scale;       /**< @brief Scale */
	glm::fquat rotation;   /**< @brief Rotation represented by a quaternion */
};

}  // namespace setsuna

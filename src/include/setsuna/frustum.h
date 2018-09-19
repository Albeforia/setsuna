#pragma once

#include <setsuna/plane.h>
#include <setsuna/aabb.h>
#include <setsuna/sphere.h>
#include <array>

/** @file
@brief Header for @ref setsuna::frustum
*/

namespace setsuna {

/**
@brief Frustum consists of six planes

The @ref #planes are stored in order of left (index `0`), right (index `1`),
top (index `2`), bottom (index `3`), near (index `4`) and far (index `5`).
*/
struct frustum {

	std::array<plane, 6> planes; /**< @brief The six planes */

public:
	/**
	@brief Default constructor, initialize to an invalid frustum
	*/
	frustum() = default;

	/**
	@brief Construct from a matrix
	*/
	explicit frustum(const glm::mat4&);

	/**
	@brief Test whether the frustum intersects a @ref setsuna::aabb

	If the aabb is inside the frustum, result will also be true.
	*/
	bool intersect(const aabb<3>&) const;

	/**
	@brief Test whether the frustum intersects a @ref setsuna::sphere

	If the sphere is inside the frustum, result will also be true.
	*/
	bool intersect(const sphere&) const;
};

}  // namespace setsuna

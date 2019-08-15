#pragma once

#include <setsuna/math/common.h>
#include <setsuna/math/plane.h>
#include <setsuna/math/aabb.h>
#include <setsuna/math/sphere.h>
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
	explicit frustum(const matrix4&);

	/**
	@brief Test whether the frustum intersects a @ref setsuna::aabb
	*/
	bool intersect(const aabb<3>&) const;

	/**
	@brief Test whether the frustum intersects a @ref setsuna::sphere
	*/
	bool intersect(const sphere&) const;
};

}  // namespace setsuna

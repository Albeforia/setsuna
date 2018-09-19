#pragma once

#include <glm/glm.hpp>
#include <array>

/** @file
@brief Header for @ref setsuna::aabb
*/

namespace setsuna {

/**
@brief Axis aligned bounding box in 2D/3D space
*/
template<uint32_t dimensions>
struct aabb {

	/**
	@brief Vertex type of the box, determined by @p dimensions
	*/
	using vertex_t = std::conditional_t<dimensions == 2, glm::vec2, glm::vec3>;

	/**
	@brief Default constructor, initialize to an invalid status
	*/
	template<typename = std::enable_if_t<dimensions == 2 || dimensions == 3>>
	aabb() { reset(); }

	/** @brief Get the center of the box */
	vertex_t center() const { return (min + max) * 0.5f; }

	/** @brief Get the extent of the box, i.e. the vector from @ref #min to the center */
	vertex_t extent() const { return (max - min) * 0.5f; }

	/**
	@brief Get the corners of the box

	@return The length of returned array is 4 if @p dimensions is 2, otherwise 8
	*/
	auto corners() const {
		std::array<vertex_t, 1 << dimensions> corners;
		auto ctr = center();
		auto ext = extent();

		for (std::size_t i = 0; i < corners.size(); ++i) {
			// use the lower 3 bits of index to represent a direction
			if constexpr (dimensions == 2) {
				auto x = (i & 1) == 0 ? 1.0f : -1.0f;
				auto y = (i & 2) == 0 ? 1.0f : -1.0f;
				corners[i] = ctr + vertex_t(x, y) * ext;
			}
			else if constexpr (dimensions == 3) {
				auto x = (i & 1) == 0 ? 1.0f : -1.0f;
				auto y = (i & 2) == 0 ? 1.0f : -1.0f;
				auto z = (i & 4) == 0 ? 1.0f : -1.0f;
				corners[i] = ctr + vertex_t(x, y, z) * ext;
			}
		}

		return corners;
	}

	/**
	@brief Expand the box so that it contains a point @p p
	*/
	void expand(const vertex_t& p) {
		min = glm::min(min, p);
		max = glm::max(max, p);
	}

	/**
	@brief Set the box to an invalid status

	Call this before recalculating @ref #min and @ref #max.
	*/
	void reset() {
		min = vertex_t(std::numeric_limits<float>::infinity());
		max = vertex_t(-std::numeric_limits<float>::infinity());
	}

public:
	vertex_t min; /**< @brief The min vertex of the box */
	vertex_t max; /**< @brief The max vertex of the box */
};

}  // namespace setsuna

#pragma once

#include <DirectXMath.h>

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
	using vertex_t = std::conditional_t<dimensions == 2, DirectX::XMFLOAT2, DirectX::XMFLOAT3>;

	/**
	@brief Default constructor, initialize to an invalid status
	*/
	template<typename = std::enable_if_t<dimensions == 2 || dimensions == 3>>
	aabb() { reset(); }

	/** @brief Get the center of the box

	@return A DirectX::XMVECTOR
	*/
	auto center() const {
		if constexpr (dimensions == 2) {
			return XMVectorScale(XMVectorAdd(
			                       XMLoadFloat2(&max), XMLoadFloat2(&min)),
			                     0.5f);
		}
		else {
			return XMVectorScale(XMVectorAdd(
			                       XMLoadFloat3(&max), XMLoadFloat3(&min)),
			                     0.5f);
		}
	}

	/** @brief Get the extent of the box, i.e. the vector from @ref #min to the center

	@return A DirectX::XMVECTOR
	*/
	auto extent() const {
		if constexpr (dimensions == 2) {
			return XMVectorScale(XMVectorSubtract(
			                       XMLoadFloat2(&max), XMLoadFloat2(&min)),
			                     0.5f);
		}
		else {
			return XMVectorScale(XMVectorSubtract(
			                       XMLoadFloat3(&max), XMLoadFloat3(&min)),
			                     0.5f);
		}
	}

	/**
	@brief Get the corners of the box

	The length of returned array is 4 if @p dimensions is 2, otherwise 8

	@return An array of DirectX::XMVECTOR
	*/
	auto corners() const {
		std::array<XMVECTOR, 1 << dimensions> corners;
		auto ctr = center();
		auto ext = extent();

		for (std::size_t i = 0; i < corners.size(); ++i) {
			// use the lower 3 bits of index to represent a direction
			if constexpr (dimensions == 2) {
				auto x = (i & 1) == 0 ? 1.0f : -1.0f;
				auto y = (i & 2) == 0 ? 1.0f : -1.0f;
				corners[i] = XMVectorMultiplyAdd(XMVectorSet(x, y, 0.0f, 0.0f),
				                                 ext,
				                                 ctr);
			}
			else if constexpr (dimensions == 3) {
				auto x = (i & 1) == 0 ? 1.0f : -1.0f;
				auto y = (i & 2) == 0 ? 1.0f : -1.0f;
				auto z = (i & 4) == 0 ? 1.0f : -1.0f;
				corners[i] = XMVectorMultiplyAdd(XMVectorSet(x, y, z, 0.0f),
				                                 ext,
				                                 ctr);
			}
		}

		return corners;
	}

	/**
	@brief Set the box to an invalid status

	Call this before recalculating @ref #min and @ref #max.
	*/
	void reset() {
		if constexpr (dimensions == 2) {
			min = vertex_t(std::numeric_limits<float>::infinity(),
			               std::numeric_limits<float>::infinity());
			max = vertex_t(-std::numeric_limits<float>::infinity(),
			               -std::numeric_limits<float>::infinity());
		}
		else {
			min = vertex_t(std::numeric_limits<float>::infinity(),
			               std::numeric_limits<float>::infinity(),
			               std::numeric_limits<float>::infinity());
			max = vertex_t(-std::numeric_limits<float>::infinity(),
			               -std::numeric_limits<float>::infinity(),
			               -std::numeric_limits<float>::infinity());
		}
	}

public:
	vertex_t min; /**< @brief The min vertex of the box */
	vertex_t max; /**< @brief The max vertex of the box */
};

}  // namespace setsuna

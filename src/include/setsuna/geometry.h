#pragma once

#include <setsuna/mesh.h>
#include <setsuna/ref.h>
#include <glm/gtc/constants.hpp>

/** @file
@brief Header for @ref setsuna::geometry
*/

namespace setsuna {

/**
@brief Factory class that produces parametric meshes

The implementation is based on @b three.js .
*/
class geometry {

public:
	/**
	@brief Create a sphere

	@param radius			Sphere radius
	@param width_segments	Number of horizontal segments
	@param height_segments	Number of vertical segments
	@param phi_length		Horizontal sweep angle size, default is 2*PI
	@param theta_length		Vertical sweep angle size, default is PI
	@param invert_y			Whether to invert vertical texture coordinates, default is true
	*/
	static ref<mesh> sphere(float radius,
	                        uint32_t width_segments, uint32_t height_segments,
	                        float phi_length = 2 * glm::pi<float>(),
	                        float theta_length = glm::pi<float>(),
	                        bool invert_y = true);

	/**
	@brief Create a plane

	@param width			Width along the X axis
	@param height			Height along the Y axis
	@param width_segments	Number of segments along the X axis, default is 1
	@param height_segments	Number of segments along the Y axis, default is 1
	@param invert_y			Whether to invert vertical texture coordinates, default is true
	*/
	static ref<mesh> plane(float width, float height,
	                       uint32_t width_segments = 1, uint32_t height_segments = 1,
	                       bool invert_y = true);
};

}  // namespace setsuna

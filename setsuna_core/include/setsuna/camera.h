#pragma once

#include <setsuna/component.h>
#include <setsuna/math/frustum.h>

/** @file
@brief Header for @ref setsuna::camera
*/

namespace setsuna {

/**
@brief Camera component

By default we use a right-handed coordinate system.
*/
class camera : public component {

	RTTI_ENABLE(camera, component)

public:
	/**
	@brief Camera type
	*/
	enum class type {
		PERSPECTIVE, /**< @brief Perspective camera */
		ORTHOGRAPHIC /**< @brief Orthographic camera */
	};

	/**
	@brief Constructor

	@param t			Type of the camera
	@param fov_or_size	Vertical field of view (in degrees) or orthographic size
	@param aspect		Aspect ratio
	@param nearp		Near plane
	@param farp			Far plane

	If @p t is @ref type::PERSPECTIVE, then @p fov_or_size is interpreted as field of view.
	Otherwise @p fov_or_size is interpreted as orthographic projection's half-size in vertical.
	*/
	camera(object3d&, type t,
	       float fov_or_size, float aspect, float nearp, float farp);

	/**
	@brief Update camera's view matrix and frustum
	*/
	void update() override;

	/**
	@brief Get the projection matrix
	*/
	matrix4 projection_matrix() const { return m_projection_matrix; }

	/**
	@brief Get the view matrix
	*/
	matrix4 view_matrix() const { return m_view_matrix; }

	/**
	@brief Get the view frustum
	*/
	const frustum& frustum() const { return m_frustum; }

	/**
	@brief Set the aspect ratio

	This will recalculate the projection matrix.
	*/
	void set_aspect(float aspect);

private:
	void update_projection();

private:
	type m_type;

	float m_fov, m_aspect, m_near_plane, m_far_plane;
	float m_orthographic_size;

	float4x4 m_projection_matrix;
	float4x4 m_view_matrix;

	setsuna::frustum m_frustum;
};

}  // namespace setsuna

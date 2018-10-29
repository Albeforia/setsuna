#pragma once

#include <setsuna/component.h>
#include <setsuna/aabb.h>
#include <setsuna/sphere.h>
#include <setsuna/material_instance.h>
#include <setsuna/ref.h>

/** @file
@brief Header for @ref setsuna::mesh_renderer
*/

namespace setsuna {

/**
@brief Mesh renderer component

Mesh renderer component holds various information needed to render a mesh.

If there is no @ref setsuna::mesh_filter component on the same object,
the mesh renderer is useless.
*/
class mesh_renderer : public component {

	RTTI_ENABLE(mesh_renderer, component)

public:
	/**
	@brief Constructor
	*/
	mesh_renderer(object3d& o3d) :
	    component(o3d) {}

	/**
	@brief Update bounding box and bounding sphere
	*/
	void update() override;

	/**
	@brief Get the bounding box in world space
	*/
	const aabb<3>& bounding_box() const { return m_aabb; }

	/**
	@brief Get the bounding sphere in world space
	*/
	const sphere& bounding_sphere() const { return m_bounding_sphere; }

public:
	/**
	@brief The material to render
	*/
	ref<material_instance> material;

private:
	// bounding box in world space
	aabb<3> m_aabb;

	// bounding sphere in world space
	sphere m_bounding_sphere;
};

}  // namespace setsuna

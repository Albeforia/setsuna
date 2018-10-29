#pragma once

#include <setsuna/component.h>
#include <setsuna/mesh.h>
#include <setsuna/ref.h>

/** @file
@brief Header for @ref setsuna::mesh_filter
*/

namespace setsuna {

/**
@brief Mesh filter component

Mesh filter component holds a reference of the mesh that will
be used by other components on the same object.

@see @ref setsuna::mesh
*/
class mesh_filter : public component {

	RTTI_ENABLE(mesh_filter, component)

public:
	/**
	@brief Constructor
	*/
	mesh_filter(object3d& o3d, ref<mesh> mesh) :
	    component(o3d), mesh(mesh){};

	ref<mesh> mesh; /**< @brief The referenced mesh */
};

}  // namespace setsuna

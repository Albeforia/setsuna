#include <setsuna/rtti_prefix.h>
#include <setsuna/mesh_renderer.h>
#include <setsuna/object3d.h>
#include <setsuna/logger.h>

namespace setsuna {

void mesh_renderer::update() {
	auto filter = m_object->get_component<mesh_filter>();
	if (filter == nullptr) {
		LOG_WARNING("The mesh_renderer requires a mesh_filter on the same object to work");
		return;
	}

	auto& world_matrix = m_object->world_matrix();
	auto corners = filter->mesh->bounding_box().corners();

	// update world space bounding box and bounding sphere
	m_aabb.reset();
	for (auto& corner : corners) {
		corner = glm::vec3(world_matrix * glm::vec4(corner, 1.0));
		m_aabb.expand(corner);
	}
	m_bounding_sphere.center = m_aabb.center();
	m_bounding_sphere.radius = glm::length(m_aabb.extent());
}

}  // namespace setsuna

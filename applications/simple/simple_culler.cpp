#include "simple_culler.h"

#include <setsuna/object3d.h>
#include <setsuna/camera.h>
#include <setsuna/mesh_filter.h>
#include <setsuna/mesh_renderer.h>

using namespace setsuna;

simple_culler::simple_culler(camera& cam, simple_culler::mode mode) :
    visitor(traversal_mode::TM_CHILDREN), m_camera{&cam}, m_cull_mode{mode} {}

void simple_culler::apply(object3d& o3d) {
	auto renderer = o3d.get_component<mesh_renderer>();
	auto filter = o3d.get_component<mesh_filter>();
	if (renderer == nullptr || filter == nullptr) return;

	bool culled = true;
	if (m_cull_mode == mode::CM_BOUNDING_BOX) {
		culled = !m_camera->frustum().intersect(renderer->bounding_box());
	}
	else {
		culled = !m_camera->frustum().intersect(renderer->bounding_sphere());
	}

	if (culled) return;

	render_queue.emplace_back(render_item{
	  o3d.world_matrix(),
	  filter->mesh,
	  renderer->material});
}

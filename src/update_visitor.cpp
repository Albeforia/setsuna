#include <setsuna/update_visitor.h>
#include <setsuna/object3d.h>

namespace setsuna {

update_visitor::update_visitor() :
    visitor(traversal_mode::TM_CHILDREN) {}

void update_visitor::apply(object3d& o3d) {
	if (o3d.parent() != nullptr && o3d.positioning != positioning_type::PT_ABSOLUTE) {
		o3d.world_matrix() = o3d.parent()->world_matrix() * glm::mat4(o3d.local_transform);
	}
	else {
		o3d.world_matrix() = glm::mat4(o3d.local_transform);
	}

	// update every component after world matrix is refreshed
	auto components = o3d.get_components<component>();
	for (auto& component : components) {
		component->update();
	}
}

}  // namespace setsuna

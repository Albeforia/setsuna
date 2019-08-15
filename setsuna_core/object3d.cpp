#include <setsuna/rtti_prefix.h>
#include <setsuna/object3d.h>

namespace setsuna {

object3d::object3d() :
    m_parent{nullptr},
    positioning{positioning_type::RELATIVE} {
	m_world_matrix = matrix4(local_transform);
}

object3d::~object3d() {
	clear_children();

	for (auto& component : m_components) {
		delete component;
	}
	m_components.clear();
}

object3d& object3d::add_child(positioning_type pt) {
	auto o3d = new object3d();
	o3d->positioning = pt;
	add_child(o3d);
	return *o3d;
}

void object3d::add_child(object3d* o3d) {
	if (o3d == nullptr) return;

	// remove from old parent
	o3d->detach();

	m_children.push_back(o3d);
	o3d->m_parent = this;
}

void object3d::detach() {
	if (m_parent != nullptr) {
		auto& siblings = m_parent->m_children;
		auto index = std::find(siblings.begin(), siblings.end(), this);
		siblings.erase(index);
		m_parent = nullptr;
	}
}

void object3d::clear_children() {
	for (auto& child : m_children) {
		delete child;
	}
	m_children.clear();
}

void object3d::accept(visitor& vis) {
	vis.apply(*this);

	if (vis.mode == traversal_mode::CHILDREN) {
		for (auto& child : m_children) {
			child->accept(vis);
			// reset mode to CHILDREN so that its neighbours won't be affected
			vis.mode = traversal_mode::CHILDREN;
		}
	}
}

}  // namespace setsuna

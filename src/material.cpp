#include <setsuna/rtti_prefix.h>
#include <setsuna/material.h>
#include <setsuna/material_instance.h>

namespace setsuna {

ref<material_instance> material::create_instance() const {
	auto mi = new material_instance(*this);
	for (auto& p : m_counter) {
		switch (p.first) {
		case SCALAR_PROPERTY:
			mi->m_scalars.resize(p.second, 0.0f);
			break;
		case COLOR_PROPERTY:
			mi->m_colors.resize(p.second, color{});
			break;
		case TEXTURE_PROPERTY:
			mi->m_textures.resize(p.second, ref<texture>{});
			break;
		default:
			break;
		}
	}
	return mi;
}

void material::add_property(property_type type, std::string_view name) {
	auto search = std::find_if(m_properties.begin(), m_properties.end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_properties.end()) {
		m_properties.emplace_back(type, name, m_counter[type]++);
	}
	// ignore duplicate properties
}

}  // namespace setsuna

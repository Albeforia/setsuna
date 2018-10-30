#include <setsuna/rtti_prefix.h>
#include <setsuna/material_instance.h>
#include <setsuna/material.h>

namespace setsuna {
material_instance::material_instance(const material& prototype) :
    m_prototype{&prototype} {};

bool material_instance::get_property(std::string_view name, float& out_value) const {
	auto search = std::find_if(m_prototype->properties_begin(),
	                           m_prototype->properties_end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_prototype->properties_end()) {
		return false;
	}

	if (search->m_type == material::SCALAR_PROPERTY) {
		out_value = m_scalars[search->m_index];
		return true;
	}

	return false;
}

bool material_instance::get_property(std::string_view name, color& out_value) const {
	auto search = std::find_if(m_prototype->properties_begin(),
	                           m_prototype->properties_end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_prototype->properties_end()) {
		return false;
	}

	if (search->m_type == material::COLOR_PROPERTY) {
		out_value = m_colors[search->m_index];
		return true;
	}

	return false;
}

bool material_instance::get_property(std::string_view name, setsuna::ref<texture>& out_value) const {
	auto search = std::find_if(m_prototype->properties_begin(),
	                           m_prototype->properties_end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_prototype->properties_end()) {
		return false;
	}

	if (search->m_type == material::TEXTURE_PROPERTY) {
		out_value = m_textures[search->m_index];
		return true;
	}

	return false;
}

void material_instance::set_property(std::string_view name, float value) {
	auto search = std::find_if(m_prototype->properties_begin(),
	                           m_prototype->properties_end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_prototype->properties_end()) {
		return;
	}

	if (search->m_type == material::SCALAR_PROPERTY) {
		m_scalars[search->m_index] = value;
	}

	return;
}

void material_instance::set_property(std::string_view name, color value) {
	auto search = std::find_if(m_prototype->properties_begin(),
	                           m_prototype->properties_end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_prototype->properties_end()) {
		return;
	}

	if (search->m_type == material::COLOR_PROPERTY) {
		m_colors[search->m_index] = value;
	}

	return;
}

void material_instance::set_property(std::string_view name, setsuna::ref<texture> value) {
	auto search = std::find_if(m_prototype->properties_begin(),
	                           m_prototype->properties_end(),
	                           [&name](const auto& p) { return p.m_name == name; });
	if (search == m_prototype->properties_end()) {
		return;
	}

	if (search->m_type == material::TEXTURE_PROPERTY) {
		m_textures[search->m_index] = value;
	}

	return;
}

}  // namespace setsuna

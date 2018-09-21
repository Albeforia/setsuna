#include <setsuna/rtti_prefix.h>
#include <setsuna/texture_manager.h>
#include <setsuna/texture_container.h>

namespace setsuna {

texture_manager::texture_manager() :
    m_option{true, 1024} {
	// get max available texture units
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_max_texture_units);

	// default texture_container
	texture_description fallback{texture_type::TEX_2D, 1, 1, texture_format::TF_RGBA8, 1};
	auto [it, _] = m_containers.try_emplace(fallback);
	auto& container = it->second.emplace_back(
	  new texture_container(fallback, 2, false, next_texture_unit()));
	auto layer = container->allocate().value();

	// fallback pure white texture
	auto white = 0xffffffff;
	m_default_texture = new ref<texture>(new texture(*container, layer));
	(*m_default_texture)->upload(0, GL_UNSIGNED_BYTE, &white);
}

texture_manager::~texture_manager() {
	// we must first destroy the default texture before delete its container
	delete m_default_texture;

	for (auto it = m_containers.begin(); it != m_containers.end(); ++it) {
		for (auto& container : it->second) {
			delete container;
		}
	}
	m_containers.clear();
}

ref<texture> texture_manager::new_texture(const texture_description& desc) {
	auto [container, layer] = next_free_layer(desc);
	return new texture(*container, layer);
}

void texture_manager::assign_unit(texture_unit unit, texture_container& container) {
	auto old_container = m_units_assignments[unit];
	if (old_container == nullptr) {
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	if (old_container != &container) {
		m_units_assignments[unit] = &container;
		glBindTextureUnit(unit, container.m_name);
	}
}

std::tuple<texture_container*, texture_layer>
texture_manager::next_free_layer(const texture_description& desc) {
	auto [it, _] = m_containers.try_emplace(desc);

	for (auto& container : it->second) {
		auto layer = container->allocate();
		if (layer) {
			return std::make_tuple(container, layer.value());
		}
	}

	// if no containers have free layers, create a new one
	auto& new_container = it->second.emplace_back(
	  new texture_container(desc, m_option.max_layers, m_option.sparse, next_texture_unit()));
	return std::make_tuple(new_container,
	                       new_container->allocate().value());
}

texture_unit texture_manager::next_texture_unit() const {
	static uint32_t cnt = 0;
	return (cnt++) % m_max_texture_units;
}

bool operator<(const texture_description& lhs, const texture_description& rhs) {
	return std::tie(lhs.type, lhs.width, lhs.height, lhs.format​, lhs.mip_levels_count) <
	       std::tie(rhs.type, rhs.width, rhs.height, rhs.format​, rhs.mip_levels_count);
}

}  // namespace setsuna

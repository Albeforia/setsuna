#include <setsuna/rtti_prefix.h>
#include <setsuna/texture_manager.h>
#include <setsuna/texture_container.h>

namespace setsuna {

texture::texture(texture_container& container, texture_layer l) :
    m_container{&container}, m_layer{l} {}

texture::~texture() {
	m_container->release(m_layer);
}

void texture::upload(GLint mip_level, GLenum data_type, const void* data) {
	m_container->upload(mip_level, m_layer, data_type, data);
}

texture_address texture::address() const {
	auto unit = m_container->unit();
	texture_manager::instance().assign_unit(unit, *m_container);
	return texture_address{unit, m_layer, {0}};
}

}  // namespace setsuna

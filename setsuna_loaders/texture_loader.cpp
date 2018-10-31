#include <setsuna_loaders/texture_loader.h>
#include <setsuna/texture_manager.h>
#include <setsuna/logger.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#define STBI_ONLY_TGA
#include <stb_image.h>

namespace setsuna {

texture_loader::texture_loader(std::string_view image_name) :
    loader(),
    m_image_name(image_name), m_data{nullptr} {}

bool texture_loader::match(const loader& other) const {
	auto p = type_cast<const texture_loader*>(&other);
	if (p != nullptr) {
		return m_image_name == p->m_image_name;
	}
	return false;
}

void texture_loader::create_resource() {
	m_texture = texture_manager::instance().default_texture();
}

void texture_loader::main_thread_stage() {
	if (m_data != nullptr) {
		m_texture = texture_manager::instance().new_texture(m_desc);
		m_texture->set_image(0, GL_UNSIGNED_BYTE, m_data);
	}

	// data is useless now
	stbi_image_free(m_data);
	m_data = nullptr;
}

void texture_loader::sub_thread_stage() {
	LOG_MESSAGE("Loading image: %s", m_image_name.c_str());

	int width, height, channels;
	m_data = stbi_load(m_image_name.c_str(), &width, &height, &channels, 0);

	if (m_data != nullptr) {
		switch (channels) {
		case 1:
			m_desc.format​ = texture_format::TF_R8;
			break;
		case 2:
			m_desc.format​ = texture_format::TF_RG8;
			break;
		case 3:
			m_desc.format​ = texture_format::TF_RGB8;
			break;
		case 4:
			m_desc.format​ = texture_format::TF_RGBA8;
			break;
		default:
			LOG_ERROR("Unsupported image format: %s", m_image_name.c_str());
			return;
		}
		m_desc.type = texture_type::TEX_2D;
		m_desc.width = width;
		m_desc.height = height;
		m_desc.mip_levels_count = 1;
	}
	else {
		LOG_ERROR("Failed to load image: %s", m_image_name.c_str());
		return;
	}

	LOG_MESSAGE("Finish loading image: %s", m_image_name.c_str());
}

}  // namespace setsuna

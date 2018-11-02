#include <setsuna/rtti_prefix.h>
#include <setsuna/framebuffer.h>
#include <setsuna/texture_container.h>

namespace setsuna {

framebuffer::framebuffer() {
	glCreateFramebuffers(1, &m_name);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_max_color_attachments);
}

framebuffer::~framebuffer() {
	glDeleteFramebuffers(1, &m_name);
}

ref<texture> framebuffer::color_attachment(std::size_t index) const {
	if (index < m_color_attachments.size()) {
		return m_color_attachments[index];
	}

	return nullptr;
}

bool framebuffer::is_complete() const {
	return glCheckNamedFramebufferStatus(m_name, GL_FRAMEBUFFER) ==
	       GL_FRAMEBUFFER_COMPLETE;
}

void framebuffer::add_attachment(ref<texture>& tex) {
	auto& desc = tex->description();

	// only support 2D texture for now
	if (desc.type != texture_type::TEX_2D) {
		LOG_WARNING("Unsupported texture type for framebuffer attachment");
		return;
	}

	switch (desc.format​) {
	case texture_format::TF_R8:
	case texture_format::TF_RG8:
	case texture_format::TF_RGB8:
	case texture_format::TF_RGBA8: {
		auto index = m_color_attachments.size();
		if (index + 1 > m_max_color_attachments) {
			LOG_WARNING("Could not add more color attachments to this framebuffer");
			return;
		}
		glNamedFramebufferTextureLayer(m_name, GL_COLOR_ATTACHMENT0 + index,
		                               tex->m_container->name(), 0, tex->m_layer);
		m_color_attachments.push_back(tex);
	} break;
	case texture_format::TF_DEPTH32:
		if (!m_depth_attachment) {
			glNamedFramebufferTextureLayer(m_name, GL_DEPTH_ATTACHMENT,
			                               tex->m_container->name(), 0, tex->m_layer);
			m_depth_attachment = tex;
		}
		else {
			return;
		}
		break;
	case texture_format::TF_DEPTH24_STENCIL:
		if (!m_depth_attachment) {
			glNamedFramebufferTextureLayer(m_name, GL_DEPTH_STENCIL_ATTACHMENT,
			                               tex->m_container->name(), 0, tex->m_layer);
			m_depth_attachment = tex;
		}
		else {
			return;
		}
		break;
	default:
		LOG_WARNING("Unsupported texture format for framebuffer attachment");
		return;
	}

	// invalidate
	tex->set_image(0, GL_UNSIGNED_BYTE, nullptr);
}

}  // namespace setsuna

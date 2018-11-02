#include <setsuna/texture_container.h>
#include <setsuna/logger.h>
#include <algorithm>
#include <array>

namespace setsuna {

texture_container::texture_container(const texture_description& desc,
                                     uint32_t layers_count, bool sparse, texture_unit unit) :
    m_desc{desc},
    m_unit{unit}, m_tile_size{0} {

	GLenum target;
	if (m_desc.type == texture_type::TEX_CUBE_MAP) {
		target = GL_TEXTURE_CUBE_MAP_ARRAY;
	}
	else {
		target = GL_TEXTURE_2D_ARRAY;
	}

	glCreateTextures(target, 1, &m_name);

	auto internalformat = static_cast<GLenum>(m_desc.format​);

	if (sparse) {
		// find the max page(or tile) size for X and Y with 1 page size for layers(Z),
		// because we commit only one layer a time
		GLint pages_count, x_size, y_size, z_size;
		GLint best_index = -1, best_x_size = 0, best_y_size = 0;

		glGetInternalformativ(target, internalformat, GL_NUM_VIRTUAL_PAGE_SIZES_ARB, 1, &pages_count);
		for (GLint i = 0; i < pages_count; ++i) {
			glTextureParameteri(m_name, GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, i);
			glGetInternalformativ(target, internalformat, GL_VIRTUAL_PAGE_SIZE_X_ARB, 1, &x_size);
			glGetInternalformativ(target, internalformat, GL_VIRTUAL_PAGE_SIZE_Y_ARB, 1, &y_size);
			glGetInternalformativ(target, internalformat, GL_VIRTUAL_PAGE_SIZE_Z_ARB, 1, &z_size);

			if (z_size == 1) {
				if (x_size >= best_x_size && y_size >= best_y_size) {
					best_index = i;
					best_x_size = x_size;
					best_y_size = y_size;
				}
			}
		}

		if (best_index == -1) {
			// there are no valid sizes or this format doesn't support sparse texture
			LOG_WARNING("Could not find suitable page size, sparse texture is disabled");
			m_sparse = false;
		}
		else {
			m_sparse = true;
			m_tile_size = best_x_size;
			glTextureParameteri(m_name, GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, best_index);
		}
	}
	else {
		m_sparse = false;
	}

	GLint max_layers;
	if (m_sparse) {
		glTextureParameteri(m_name, GL_TEXTURE_SPARSE_ARB, GL_TRUE);
		glGetIntegerv(GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB, &max_layers);
	}
	else {
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
	}

	m_layers_count = std::min(layers_count, (uint32_t)max_layers);

	// for cubemap array, one layer is corresponding to 6 layer-faces
	if (m_desc.type == texture_type::TEX_CUBE_MAP) {
		m_layers_count /= 6;
	}

	// if TEXTURE_SPARSE_ARB is TRUE, only virtual address space is
	// allocated for the texture but physical backing store is not
	glTextureStorage3D(m_name, m_desc.mip_levels_count, internalformat,
	                   m_desc.width, m_desc.height,
	                   m_desc.type == texture_type::TEX_CUBE_MAP
	                     ? 6 * m_layers_count
	                     : m_layers_count);

	glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTextureParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// mark all layers free
	for (texture_layer l = 0; l < m_layers_count; ++l) {
		m_free_list.push(l);
	}
}

texture_container::~texture_container() {
	if (m_free_list.size() != m_layers_count) {
		LOG_DEBUG("A texture_container is being destructed but it still has unreleased textures");
	}

	glDeleteTextures(1, &m_name);
}

std::optional<texture_layer> texture_container::allocate() {
	if (m_free_list.size() > 0) {
		auto layer = m_free_list.front();
		m_free_list.pop();
		//commit_or_free(layer, true);
		return layer;
	}
	return std::nullopt;
}

void texture_container::release(texture_layer l) {
	m_free_list.push(l);
	commit_or_free(l, false);
}

void texture_container::upload(GLint mip_level, texture_layer l,
                               GLenum data_type, const void* data) {
	commit_or_free(l, true);

	// NOTE NULL data cannot be used to invalidate immutable storage
	// note glTexSubImage3D needs a BASE internal format while
	// glTexStorage3D needs a SIZED internal format
	if (m_desc.type == texture_type::TEX_CUBE_MAP) {
		auto actual_data = static_cast<const std::array<void*, 6>*>(data);
		// upload each face in the layer
		for (int i = 0; i < 6; ++i) {
			if (data == nullptr || (*actual_data)[i] == nullptr) {
				glInvalidateTexSubImage(m_name, mip_level, 0, 0, 6 * l + i,
				                        m_desc.width, m_desc.height, 1);
			}
			else {
				glTextureSubImage3D(m_name, mip_level, 0, 0, 6 * l + i,
				                    m_desc.width, m_desc.height, 1, sized_to_base(m_desc.format​),
				                    data_type, (*actual_data)[i]);
			}
		}
	}
	else {
		if (data == nullptr) {
			glInvalidateTexSubImage(m_name, mip_level, 0, 0, l,
			                        m_desc.width, m_desc.height, 1);
		}
		else {
			glTextureSubImage3D(m_name, mip_level, 0, 0, l,
			                    m_desc.width, m_desc.height, 1, sized_to_base(m_desc.format​),
			                    data_type, data);
		}
	}
}

void texture_container::commit_or_free(texture_layer l, bool commit) {
	if (!m_sparse) return;

	auto width = m_desc.width;
	auto height = m_desc.height;

	GLenum target;
	GLint zoffset;
	GLsizei depth;
	if (m_desc.type == texture_type::TEX_CUBE_MAP) {
		target = GL_TEXTURE_CUBE_MAP_ARRAY;
		zoffset = 6 * l;
		depth = 6;
	}
	else {
		target = GL_TEXTURE_2D_ARRAY;
		zoffset = l;
		depth = 1;
	}

	// for now texture commitment cannot use DSA
	glBindTexture(target, m_name);

	// commit(or de-commit) all mip levels
	for (GLint level = 0; level < m_desc.mip_levels_count; ++level) {
		glTexPageCommitmentARB(target, level, 0, 0, zoffset,
		                       width, height, depth, (GLboolean)commit);
		width = std::max(1, width / 2);
		height = std::max(1, height / 2);
	}

	glBindTexture(target, 0);
}

}  // namespace setsuna

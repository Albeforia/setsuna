#pragma once

#include <setsuna/texture_property.h>
#include <queue>
#include <optional>

namespace setsuna {

/*
This class stores textures of the same texture_description
i.e. same type, size, internalformat and mip levels​.

This class is effectively an encapsulation of OpenGL texture array
with every layer representing a 2D/cube texture, and able to utilize
sparse texture when possible.
*/
class texture_container {

	friend class texture_manager;

public:
	~texture_container();

	texture_container(const texture_container&) = delete;
	texture_container& operator=(const texture_container&) = delete;

	std::optional<texture_layer> allocate();

	void release(texture_layer);

	texture_unit unit() const { return m_unit; }

	void upload(GLint mip_level, texture_layer, GLenum data_type, const void* data);

private:
	// only called by texture_manager
	texture_container(const texture_description&, uint32_t layers_count,
	                  bool sparse, texture_unit);

	void commit_or_free(texture_layer, bool commit);

private:
	GLuint m_name;
	texture_unit m_unit;

	texture_description m_desc;
	uint32_t m_layers_count;
	bool m_sparse;
	GLsizei m_tile_size;

	std::queue<texture_layer> m_free_list;
};

}  // namespace setsuna

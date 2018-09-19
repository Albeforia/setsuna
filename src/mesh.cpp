#include <setsuna/rtti_prefix.h>

namespace setsuna {

mesh::mesh() :
    m_vertices_count{0}, m_indices_count{0} {
	glCreateVertexArrays(1, &m_vao);
}

mesh::~mesh() {
	glDeleteVertexArrays(1, &m_vao);
}

void mesh::render() {
	glBindVertexArray(m_vao);

	if (m_indices_count > 0) {
		glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, m_vertices_count);
	}

	// unbind in case that we delete these buffers later ?
}

void mesh::set_indices(const std::vector<uint32_t>& indices) {
	buffer<buffer_usage::BU_STATIC> new_index_buffer;
	new_index_buffer.create(indices);
	m_index_buffer = std::move(new_index_buffer);
	if (m_index_buffer.empty()) {
		glVertexArrayElementBuffer(m_vao, 0);
		m_indices_count = 0;
	}
	else {
		glVertexArrayElementBuffer(m_vao, m_index_buffer.name());
		m_indices_count = indices.size();
	}
}

void mesh::calculate_bounding_box(const std::vector<glm::vec3>& vertices) {
	m_aabb.reset();
	for (auto& v : vertices) {
		m_aabb.expand(v);
	}
}

}  // namespace setsuna

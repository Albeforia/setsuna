#pragma once

#include <glad/glad.h>

#include <setsuna/resource.h>
#include <setsuna/ref.h>
#include <setsuna/buffer.h>
#include <setsuna/aabb.h>

#include <vector>
#include <algorithm>

/** @file
@brief Header for @ref setsuna::mesh
*/

namespace setsuna {

/**
@brief Mesh attribute

@see @ref setsuna::mesh
*/
template<typename T>
struct attribute {

	static constexpr auto size_bytes = sizeof(T);

	// TODO replace GLenum with enum class
	GLenum type;         /**< @brief Type of the elementary data, e.g. @p GL_FLOAT */
	uint32_t components; /**< @brief Number of elementary data per attribute */
	std::vector<T> data; /**< @brief Data of the attribute */

	/**
	@brief Constructor

	If you do not want the attribute to duplicate @p data,
	use @p std::move, for example:

	@code{.cpp}
	std::vector<glm::vec3> positions{
		{-1.0, -1.0, 0.0},
		{ 1.0, -1.0, 0.0},
		{ 0.0,  1.0, 0.0}
	};
	attribute attr(3, GL_FLOAT, std::move(positions));
	@endcode
	*/
	attribute(uint32_t components, GLenum type, std::vector<T> data) :
	    type{type}, components{components}, data(std::move(data)) {}
};

/*
Calculate the stride of the vertex(if attributes are interleaved) and
determine the number of vertices, ignoring empty attributes.
*/
template<typename Attribute, typename... Attributes>
std::size_t attributes_stride(std::size_t& min_count,
                              const Attribute& attr,
                              const Attributes&... attrs) {
	min_count = attr.data.empty() ? min_count : std::min(attr.data.size(), min_count);
	if constexpr (sizeof...(Attributes) == 0) {
		return attr.data.empty() ? 0 : Attribute::size_bytes;
	}
	else {
		return (attr.data.empty()
		          ? 0
		          : Attribute::size_bytes) +
		       attributes_stride(min_count, attrs...);
	}
}

/**
@brief Static mesh
*/
class mesh : public resource {

	RTTI_ENABLE(mesh, resource)

public:
	/**
	@brief Construct a new mesh

	@param interleaved	Whether the attributes are interleaved
	@param attr			The first attribute
	@param attrs		The remaining attributes

	@see @ref setsuna::attribute
	*/
	template<typename Attribute, typename... Attributes>
	static setsuna::ref<mesh> create(bool interleaved,
	                                 const Attribute& attr,
	                                 const Attributes&... attrs) {
		auto count = std::numeric_limits<std::size_t>::max();
		auto stride = attributes_stride(count, attr, attrs...);

		auto new_mesh = new mesh();
		std::vector<uint8_t> data(stride * count);
		uint32_t offset = 0;
		if (interleaved) {
			create_interleaved_impl<0>(*new_mesh, data, offset, count, stride, attr, attrs...);
		}
		else {
			create_impl<0>(*new_mesh, data, offset, count, attr, attrs...);
		}

		new_mesh->m_vertex_buffer.create(data);
		new_mesh->m_vertices_count = count;
		return new_mesh;
	}

private:
	template<uint32_t attribindex, typename Attribute, typename... Attributes>
	static void create_impl(mesh& mesh,
	                        std::vector<uint8_t>& data,
	                        uint32_t& offset,
	                        std::size_t count,
	                        const Attribute& attr,
	                        const Attributes&... attrs) {
		if (!attr.data.empty()) {  // skip empty attributes
			auto src = reinterpret_cast<const uint8_t*>(attr.data.data());
			std::copy(src, src + Attribute::size_bytes * count, data.begin() + offset);
			glVertexArrayAttribFormat(mesh.m_vao, attribindex,
			                          attr.components, attr.type, GL_FALSE, offset);
			glEnableVertexArrayAttrib(mesh.m_vao, attribindex);
			glVertexArrayVertexBuffer(mesh.m_vao, attribindex,
			                          mesh.m_vertex_buffer.name(), 0, Attribute::size_bytes);
			offset += Attribute::size_bytes * count;
		}
		if constexpr (sizeof...(Attributes) != 0) {
			create_impl<attribindex + 1>(mesh, data, offset, count, attrs...);
		}
	}

	template<uint32_t attribindex, typename Attribute, typename... Attributes>
	static void create_interleaved_impl(mesh& mesh,
	                                    std::vector<uint8_t>& data,
	                                    uint32_t& offset,
	                                    std::size_t count,
	                                    std::size_t stride,
	                                    const Attribute& attr,
	                                    const Attributes&... attrs) {
		if (!attr.data.empty()) {  // skip empty attributes
			for (uint32_t i = 0; i < count; ++i) {
				auto src = reinterpret_cast<const uint8_t*>(attr.data.data()) +
				           i * Attribute::size_bytes;
				std::copy(src, src + Attribute::size_bytes,
				          data.begin() + offset + i * stride);
			}
			glVertexArrayAttribFormat(mesh.m_vao, attribindex,
			                          attr.components, attr.type, GL_FALSE, offset);
			glEnableVertexArrayAttrib(mesh.m_vao, attribindex);
			glVertexArrayVertexBuffer(mesh.m_vao, attribindex,
			                          mesh.m_vertex_buffer.name(), 0, stride);
			offset += Attribute::size_bytes;
		}
		if constexpr (sizeof...(Attributes) != 0) {
			create_interleaved_impl<attribindex + 1>(mesh, data, offset, count, stride, attrs...);
		}
	}

public:
	/**
	@brief Destructor
	*/
	~mesh();

	/**
	@brief Copying is not allowed
	*/
	mesh(const mesh&) = delete;

	mesh& operator=(const mesh&) = delete;

	// TODO move

	/**
	@brief Render the mesh
	*/
	void render();

	/**
	@brief Set mesh indices

	If @p indices is empty, then the previously set indices will be canceled
	and the mesh will not use indices while rendering.

	Currently only support 32-bit integer as index type.
	*/
	void set_indices(const std::vector<uint32_t>& indices);

	/**
	@brief Calculate the bounding box in model space
	*/
	void calculate_bounding_box(const std::vector<glm::vec3>& vertices);

	/**
	@brief Get the bounding box in model space
	*/
	const aabb<3>& bounding_box() const { return m_aabb; }

private:
	mesh();

	GLuint m_vao;
	buffer<buffer_usage::BU_STATIC> m_vertex_buffer;
	buffer<buffer_usage::BU_STATIC> m_index_buffer;

	uint32_t m_vertices_count;
	uint32_t m_indices_count;

	aabb<3> m_aabb;
};

}  // namespace setsuna

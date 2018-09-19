#pragma once

#include <glad/glad.h>
#include <setsuna/logger.h>
#include <vector>

/** @file
@brief Header for @ref setsuna::buffer
*/

namespace setsuna {

/**
@brief Buffer usage
*/
enum class buffer_usage {
	BU_STATIC,    /**< @brief Static buffer, data cannot be updated after creation */
	BU_DYNAMIC,   /**< @brief Dynamic buffer, data can be updated after creation */
	BU_PERSISTENT /**< @brief Persistently mapped buffer */
};

/**
@brief Buffer object with immutable data store

@tparam usage @ref setsuna::buffer_usage
*/
template<buffer_usage usage>
class buffer {

public:
	/**
	@brief Default constructor
	*/
	buffer() :
	    m_created{false}, m_data{nullptr} {
		glCreateBuffers(1, &m_name);
	}

	/**
	@brief Destructor
	*/
	~buffer() {
		if (m_name == 0) return;

		if (m_data != nullptr) {
			glUnmapNamedBuffer(m_name);
			m_data = nullptr;
		}
		glDeleteBuffers(1, &m_name);
	}

	/**
	@brief Copying is not allowed
	*/
	buffer(const buffer&) = delete;

	buffer& operator=(const buffer&) = delete;

	/**
	@brief Move-constructor
	*/
	buffer(buffer&& other) noexcept :
	    m_name{other.m_name}, m_created{other.m_created}, m_data{other.m_data} {
		other.m_name = 0;
		other.m_created = false;
		other.m_data = nullptr;
	}

	/**
	@brief Move assignment
	*/
	buffer& operator=(buffer&& other) noexcept {
		if (this != &other) {
			if (m_name != 0) {
				if (m_data != nullptr) {
					glUnmapNamedBuffer(m_name);
					m_data = nullptr;
				}
				glDeleteBuffers(1, &m_name);
			}

			m_name = other.m_name;
			m_created = other.m_created;
			m_data = other.m_data;
			other.m_name = 0;
			other.m_created = false;
			other.m_data = nullptr;
		}
		return *this;
	}

	/**
	@brief Create the buffer

	@param data Initial data of the buffer

	The buffer will not be created if @p data is empty.
	To create a buffer with uninitialized data store, use @ref create(std::size_t) instead.
	*/
	template<typename T>
	void create(const std::vector<T>& data) {
		if (!m_created && !data.empty()) {
			create(sizeof(T) * data.size(), data.data());
		}
	}

	/**
	@brief Create the buffer with uninitialized data store

	@param size Number of elements

	The buffer will not be created if @p size is zero.

	This function is not available if @p usage is @ref setsuna::buffer_usage::BU_STATIC .
	*/
	template<typename T,
	         typename = std::enable_if_t<usage == buffer_usage::BU_DYNAMIC || usage == buffer_usage::BU_PERSISTENT>>
	void create(std::size_t size) {
		if (!m_created && size > 0) {
			create(sizeof(T) * size, nullptr);
		}
	}

	/**
	@brief Set buffer data

	@param data		The data to upload
	@param offset	Offset in bytes

	You should call @ref create() or @ref create(std::size_t) at least once to
	ensure the buffer is created properly, otherwise this function will do nothing.

	This function is not available if @p usage is @ref setsuna::buffer_usage::BU_STATIC .
	*/
	template<typename T,
	         typename = std::enable_if_t<usage == buffer_usage::BU_DYNAMIC || usage == buffer_usage::BU_PERSISTENT>>
	void set_data(const std::vector<T>& data, GLintptr offset) {
		if (m_created && !data.empty()) {
			update(sizeof(T) * data.size(), offset, data.data());
		}
	}

	/**
	@brief Test if the buffer is empty

	A buffer with uninitialized data store is considered non-empty.
	*/
	bool empty() const { return !m_created; }

	/**
	@brief Get the name of the buffer
	*/
	GLuint name() const { return m_name; }

private:
	void create(GLsizeiptr size, const void* data) {
		// re-create if moved-out
		if (m_name == 0) glCreateBuffers(1, &m_name);

		if constexpr (usage == buffer_usage::BU_STATIC) {
			glNamedBufferStorage(m_name, size, data, 0);
		}
		else if constexpr (usage == buffer_usage::BU_DYNAMIC) {
			glNamedBufferStorage(m_name, size, data, GL_DYNAMIC_STORAGE_BIT);
		}
		else if constexpr (usage == buffer_usage::BU_PERSISTENT) {
			// map for writing only and implies coherent (for now)
			GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
			glNamedBufferStorage(m_name, size, data, flags);
			// map only once
			m_data = reinterpret_cast<uint8_t*>(glMapNamedBufferRange(m_name, 0, size, flags));
			if (m_data == nullptr) {
				LOG_ERROR("Map buffer failed");
			}
		}
		m_created = true;
	}

	void update(GLsizeiptr size, GLintptr offset, const void* data) {
		if constexpr (usage == buffer_usage::BU_DYNAMIC) {
			glNamedBufferSubData(m_name, offset, size, data);
		}
		else if constexpr (usage == buffer_usage::BU_PERSISTENT) {
			memcpy(m_data + offset, data, size);
		}
	}

private:
	GLuint m_name;
	bool m_created;
	uint8_t* m_data;
};

}  // namespace setsuna

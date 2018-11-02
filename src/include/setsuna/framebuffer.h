#pragma once

#include <glad/glad.h>
#include <setsuna/texture.h>
#include <setsuna/ref.h>
#include <vector>

/** @file
@brief Header for @ref setsuna::framebuffer
*/

namespace setsuna {

/**
@brief Framebuffer
*/
class framebuffer {

public:
	/**
	@brief Default constructor
	*/
	framebuffer();

	/**
	@brief Destructor
	*/
	~framebuffer();

	/**
	@brief Copying is not allowed
	*/
	framebuffer(const framebuffer&) = delete;

	framebuffer& operator=(const framebuffer&) = delete;

	/**
	@brief Add texture attachments
	*/
	template<typename T, typename... Ts>
	void add_attachments(T attachment, Ts... remains) {
		add_attachment(attachment);
		if constexpr (sizeof...(remains) > 0) {
			add_attachments(remains...);
		}
	}

	/**
	@brief Get the name of the framebuffer
	*/
	GLuint name() const { return m_name; }

	/**
	@brief Get the color attachment indexed by @p index
	*/
	ref<texture> color_attachment(std::size_t index) const;

	/**
	@brief Get the depth attachment
	*/
	ref<texture> depth_attachment() const { return m_depth_attachment; }

	/**
	@brief Check if framebuffer is complete
	*/
	bool is_complete() const;

private:
	void add_attachment(ref<texture>&);

private:
	GLuint m_name;

	std::vector<ref<texture>> m_color_attachments;

	ref<texture> m_depth_attachment;

	GLsizei m_max_color_attachments;
};

}  // namespace setsuna

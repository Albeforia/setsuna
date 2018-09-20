#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

/** @file
@brief Header for @ref setsuna::shader_program
*/

namespace setsuna {

/*
Just a workaround so that we can find uniform by string_view
*/
struct uniform_name {

	uniform_name(std::string_view);

	std::size_t hash;
};
}  // namespace setsuna

namespace std {
template<>
struct hash<setsuna::uniform_name> {
	size_t operator()(const setsuna::uniform_name& un) const {
		return un.hash;
	}
};

}  // namespace std

namespace setsuna {

/**
@brief Shader type
*/
enum class shader_type {
	ST_VERTEX,  /**< @brief Vertex shader */
	ST_FRAGMENT /**< @brief Fragment shader */
};

/**
@brief Shader program
*/
class shader_program {

public:
	/**
	@brief Default constructor
	*/
	shader_program();

	/**
	@brief Destructor
	*/
	~shader_program();

	/**
	@brief Copying is not allowed
	*/
	shader_program(const shader_program&) = delete;

	shader_program& operator=(const shader_program&) = delete;

	// TODO move

	/**
	@brief Add a shader
	*/
	void add_shader(shader_type, std::string_view filename);

	/**
	@brief Compile the added shaders

	Note regardless of success or not, the added shaders will be deleted.

	Though OpenGL allows relinking the program, call this on a successfully compiled
	program will not recompile it. Just create another program instead.
	*/
	void compile();

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, int);

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, float);

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, bool);

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, const glm::vec2&);

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, const glm::vec3&);

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, const glm::mat3&);

	/**
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, const glm::mat4&);

	/**
	@brief Use the shader program
	*/
	void apply();

private:
	bool read_file(std::string_view filename, std::string& content);

	GLuint create_shader(GLenum target, const std::string& source);

	GLint get_uniform_location(std::string_view& name);

private:
	GLuint m_program;
	std::vector<GLuint> m_shaders;
	std::unordered_map<uniform_name, GLint> m_uniforms;

	std::unordered_set<std::string> m_includes;  // glsl 'header guard'
};

bool operator==(const setsuna::uniform_name& lhs, const setsuna::uniform_name& rhs);

}  // namespace setsuna

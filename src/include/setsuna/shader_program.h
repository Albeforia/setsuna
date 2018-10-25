#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>
#include <string_view>
#include <map>
#include <unordered_set>

/** @file
@brief Header for @ref setsuna::shader_program
*/

namespace setsuna {

struct texture_address;

/**
@brief Shader type
*/
enum class shader_type {
	ST_VERTEX,  /**< @brief Vertex shader */
	ST_FRAGMENT /**< @brief Fragment shader */
};

/*
A uniform variable of struct type can take up multiple locations
*/
struct uniform_info {

	std::string type;
	std::vector<GLint> locations;
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
	@brief Upload uniform variable
	*/
	void upload_uniform(std::string_view name, const texture_address&);

	/**
	@brief Use the shader program
	*/
	void apply();

private:
	bool read_file(std::string_view filename, std::string& content);

	GLuint create_shader(GLenum target, const std::string& source);

	GLint* get_uniform_location(std::string_view& name);

private:
	GLuint m_program;
	std::vector<GLuint> m_shaders;

	// specify a transparent comparator so that we can lookup string by string_view
	std::map<std::string, uniform_info, std::less<>> m_uniforms;

	// glsl 'header guard'
	std::unordered_set<std::string> m_includes;
};

}  // namespace setsuna

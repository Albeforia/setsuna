#include <setsuna/shader_program.h>
#include <setsuna/logger.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <regex>

namespace setsuna {

void shader_program::add_shader(shader_type type, std::string_view filename) {
	std::string src;

	// clear old header guards
	m_includes.clear();
	if (!read_file(filename, src)) {
		LOG_ERROR("Could not read shader file: %s", filename)
	}

	switch (type) {
	case shader_type::ST_VERTEX:
		m_shaders.push_back(create_shader(GL_VERTEX_SHADER, src));
		break;
	case shader_type::ST_FRAGMENT:
		m_shaders.push_back(create_shader(GL_FRAGMENT_SHADER, src));
		break;
	default:
		break;
	}
}

bool shader_program::read_file(std::string_view filename, std::string& content) {
	static std::regex r_include("#include\\s*<([A-Za-z0-9_]+\\.glsl)>");

	// TODO string_view::data may return a non-null-terminated string
	std::ifstream f(filename.data());

	bool success = true;
	if (f.is_open()) {
		// current directory
		auto pos = filename.find_last_of('/');
		std::string dir(filename.substr(0, pos == std::string_view::npos ? 0 : pos + 1));

		std::smatch match;
		std::string line;
		while (getline(f, line)) {
			if (std::regex_match(line, match, r_include)) {
				auto inc = match[1].str();
				if (m_includes.find(inc) == m_includes.end()) {
					m_includes.insert(inc);
					std::string included_file = dir + inc;
					std::string included_src;
					// include recursively
					if (read_file(included_file, included_src)) {
						content.append(included_src);
					}
					else {
						LOG_ERROR("Could not include shader file: %s", inc.c_str());
						success = false;
						break;
					}
				}
			}
			else {
				content.append(line + "\n");
			}
		}
		f.close();
	}
	else {
		success = false;
	}

	return success;
}

GLuint shader_program::create_shader(GLenum target, const std::string& source) {
	auto shader = glCreateShader(target);
	auto srcraw = source.c_str();
	glShaderSource(shader, 1, &srcraw, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infolen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);
		auto info = new GLchar[infolen + 1];
		glGetShaderInfoLog(shader, infolen, nullptr, info);
		LOG_ERROR("Could not compile shader:\n%s", info);
		delete[] info;
	}

	return shader;
}

void shader_program::compile() {
	auto program = glCreateProgram();
	for (std::size_t i = 0; i < m_shaders.size(); i++) {
		glAttachShader(program, m_shaders[i]);
	}
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infolen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infolen);
		auto info = new GLchar[infolen + 1];
		glGetProgramInfoLog(program, infolen, nullptr, info);
		LOG_ERROR("Could not link program:\n%s", info);
		delete[] info;
	}

	for (std::size_t i = 0; i < m_shaders.size(); i++) {
		glDetachShader(program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	m_shaders.clear();

	m_program = program;
}

GLint shader_program::get_uniform_location(std::string_view& name) {
	GLint location;
	uniform_name uname(name);
	auto search = m_uniforms.find(uname);
	if (search != m_uniforms.end()) {
		location = m_uniforms[uname];
	}
	else {
		location = glGetUniformLocation(m_program, name.data());
		if (location != 0xFFFFFFFF) {
			m_uniforms[uname] = location;
		}
		else {
			LOG_WARNING("The uniform variable \"%s\" doesn't exist", name.data())
		}
	}
	return location;
}

void shader_program::upload_uniform(std::string_view name, int value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniform1i(location, value);
	}
}

void shader_program::upload_uniform(std::string_view name, float value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniform1f(location, value);
	}
}

void shader_program::upload_uniform(std::string_view name, bool value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniform1i(location, value);
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::vec2& value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniform2f(location, value.x, value.y);
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::vec3& value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniform3f(location, value.x, value.y, value.z);
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::mat3& value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::mat4& value) {
	auto location = get_uniform_location(name);
	if (location != 0xFFFFFFFF) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

void shader_program::apply() {
#ifdef _DEBUG
	glValidateProgram(m_program);
	GLint legality;
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &legality);
	if (legality == GL_FALSE) {
		GLint infolen;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infolen);
		auto info = new GLchar[infolen + 1];
		glGetProgramInfoLog(m_program, infolen, nullptr, info);
		LOG_ERROR("Shader program is invalid:\n%s", info);
		delete[] info;
	}
#endif
	glUseProgram(m_program);
}

uniform_name::uniform_name(std::string_view name) :
    hash{std::hash<std::string_view>{}(name)} {}

/*
This is right iff we assume no hash collision
*/
bool operator==(const setsuna::uniform_name& lhs, const setsuna::uniform_name& rhs) {
	return lhs.hash == rhs.hash;
}

}  // namespace setsuna

#include <setsuna/shader_program.h>
#include <setsuna/texture_property.h>
#include <setsuna/logger.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <regex>

#define INVALID_LOCATION 0xFFFFFFFF

namespace setsuna {

shader_program::shader_program() :
    m_program{0} {}

shader_program::~shader_program() {
	for (std::size_t i = 0; i < m_shaders.size(); i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	m_shaders.clear();

	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

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
	static std::regex r_include(R"(#include\s*<([A-Za-z0-9_/]+\.glh)>)");
	static std::regex r_uniform(R"((?:layout\(.*\))?\s*uniform\s+(\w+)\s+(\w+)(?:\[\d+\])?\s*;)");

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
			else if (std::regex_match(line, match, r_uniform)) {
				const auto& utype = match[1].str();
				const auto& uname = match[2].str();
				if (utype == "int" || utype == "float" ||
				    utype == "vec2" || utype == "vec3" ||
				    utype == "mat3" || utype == "mat4" ||
				    utype == "Tex2D") {
					m_uniforms.emplace(uname, uniform_info{}).first->second.type = utype;
				}
				else {
					LOG_WARNING("Unsupported uniform type: %s", utype.c_str());
				}
				content.append(line + "\n");
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
	if (m_program != 0) {
		LOG_WARNING("Recompiling the program is not allowed");
		return;
	}

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

	if (status == GL_FALSE) {
		glDeleteProgram(program);
		m_program = 0;
	}
	else {
		m_program = program;

		// fill in all uniform locations
		for (auto it = m_uniforms.begin(); it != m_uniforms.end();) {
			auto& type = it->second.type;
			if (type == "Tex2D") {
				auto unit_name = it->first + ".unit";
				auto layer_name = it->first + ".layer";
				auto unit_loc = glGetUniformLocation(m_program, unit_name.c_str());
				auto layer_loc = glGetUniformLocation(m_program, layer_name.c_str());
				if (unit_loc == INVALID_LOCATION || layer_loc == INVALID_LOCATION) {
					it = m_uniforms.erase(it);
				}
				else {
					it->second.locations.push_back(unit_loc);
					it->second.locations.push_back(layer_loc);
					++it;
				}
			}
			else {
				auto loc = glGetUniformLocation(m_program, it->first.c_str());
				if (loc == INVALID_LOCATION) {
					it = m_uniforms.erase(it);
				}
				else {
					it->second.locations.push_back(loc);
					++it;
				}
			}
		}
	}
}

GLint* shader_program::get_uniform_location(std::string_view& name) {
	if (m_program == 0) return nullptr;

	GLint* locations = nullptr;
	auto search = m_uniforms.find(name);  // O(log(n))
	if (search != m_uniforms.end()) {
		locations = search->second.locations.data();
	}
	else {
		LOG_WARNING("The uniform variable \"%s\" doesn't exist", name.data())
	}
	return locations;
}

void shader_program::upload_uniform(std::string_view name, int value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniform1i(*location, value);
	}
}

void shader_program::upload_uniform(std::string_view name, float value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniform1f(*location, value);
	}
}

void shader_program::upload_uniform(std::string_view name, bool value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniform1i(*location, value);
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::vec2& value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniform2f(*location, value.x, value.y);
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::vec3& value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniform3f(*location, value.x, value.y, value.z);
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::mat3& value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniformMatrix3fv(*location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

void shader_program::upload_uniform(std::string_view name, const glm::mat4& value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniformMatrix4fv(*location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

void shader_program::upload_uniform(std::string_view name, const texture_address& value) {
	auto location = get_uniform_location(name);
	if (location != nullptr) {
		glUniform1i(location[0], value.unit);
		glUniform1i(location[1], value.layer);
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

}  // namespace setsuna

#pragma once

#include <glad/glad.h>
#include <cstdint>

/** @file
@brief Texture related properties

@see @ref setsuna::texture_type @ref setsuna::texture_format
@ref setsuna::texture_unit @ref setsuna::texture_layer
*/

namespace setsuna {

/**
@brief Texture unit
*/
using texture_unit = GLuint;

/**
@brief Texture layer

Every @ref setsuna::texture is actually located at one layer of some container.
The combination of @p texture_layer and @ref setsuna::texture_unit forms the address
of a texture i.e. @ref setsuna::texture_address .
*/
using texture_layer = uint32_t;

/**
@brief Texture address used in shader
*/
struct texture_address {
	texture_unit unit;   /**< @brief Texture unit */
	texture_layer layer; /**< @brief Texture layer */
	GLint _reserved[2];
};

/**
@brief Texture type
*/
enum class texture_type : GLenum {
	TEX_2D = GL_TEXTURE_2D,            /**< @brief 2D texture */
	TEX_CUBE_MAP = GL_TEXTURE_CUBE_MAP /**< @brief Cubemap texture */
};

/**
@brief Texture format
*/
enum class texture_format : GLenum {
	TF_R8 = GL_R8,
	TF_RG8 = GL_RG8,
	TF_RGB8 = GL_RGB8,
	TF_RGBA8 = GL_RGBA8,
	TF_DEPTH32 = GL_DEPTH_COMPONENT32
};

/**
@brief Texture description
*/
struct texture_description {
	texture_type type;        /**< @brief Type of the texture */
	GLsizei width;            /**< @brief Width of the texture */
	GLsizei height;           /**< @brief Height of the texture */
	texture_format format​; /**< @brief Format of the texture */
	GLsizei mip_levels_count; /**< @brief Total mipmap levels of the texture */
};

/*
Convert a sized format to the corresponding base format.
*/
constexpr GLenum sized_to_base(texture_format format) {
	switch (format) {
	case texture_format::TF_R8:
		return GL_RED;
	case texture_format::TF_RG8:
		return GL_RG;
	case texture_format::TF_RGB8:
		return GL_RGB;
	case texture_format::TF_RGBA8:
		return GL_RGBA;
	case texture_format::TF_DEPTH32:
		return GL_DEPTH_COMPONENT;
	default:
		return GL_NONE;
	}
}

}  // namespace setsuna

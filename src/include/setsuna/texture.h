#pragma once

#include <setsuna/resource.h>
#include <setsuna/texture_property.h>

/** @file
@brief Header for @ref setsuna::texture
*/

namespace setsuna {

class texture_container;

/**
@brief Texture

Use @ref setsuna::texture_manager to create a texture.
*/
class texture : public resource {

	friend class texture_manager;
	friend class framebuffer;

	RTTI_ENABLE(texture, resource)

public:
	/**
	@brief Destructor
	*/
	~texture();

	/**
	@brief Copying is not allowed
	*/
	texture(const texture&) = delete;

	texture& operator=(const texture&) = delete;

	// TODO move

	/**
	@brief Set image data

	@param mip_level	The mip level to set
	@param data_type	The type of data, e.g. @p GL_UNSIGNED_BYTE
	@param data			The image data

	If this is a 2D texture, @p data is interpreted as @p void* .

	If this is a cubemap, @p data is interpreted as @p array<void*, 6>* .

	Specify @p data as @p nullptr to invalidate the texture storage.
	*/
	void set_image(GLint mip_level, GLenum data_type, const void* data);

	/**
	@brief Get the texture description
	*/
	const texture_description& description() const;

	/**
	@brief Get the texture address
	*/
	texture_address address() const;

private:
	// only called by texture_manager
	texture(texture_container&, texture_layer);

	// a reference to the texture_container that contains this texture
	texture_container* m_container;

	texture_layer m_layer;
};

}  // namespace setsuna

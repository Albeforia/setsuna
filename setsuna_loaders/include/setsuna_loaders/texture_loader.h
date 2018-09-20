#pragma once

#include <setsuna/loader.h>
#include <setsuna/texture.h>
#include <setsuna/ref.h>
#include <string>
#include <string_view>

/** @file
@brief Header for @ref setsuna::texture_loader
*/

namespace setsuna {

/**
@brief Texture loader based on stb image

Supported formats: 8bit per-channel JPEG/PNG/BMP/TGA
*/
class texture_loader : public loader {

	RTTI_ENABLE(texture_loader, loader)

public:
	/**
	@brief Constructor

	@param image_name Filename of the image
	*/
	texture_loader(std::string_view image_name);

	/**
	@brief Destructor
	*/
	~texture_loader() {}

	void create_resource() override;

	void sub_thread_stage() override;

	void main_thread_stage() override;

	bool match(const loader&) const override;

public:
	/**
	@brief Get the loading result

	@return Return the loaded texture if loading has finished,
	otherwise return the fallback texture
	*/
	ref<texture> get() const { return m_texture; }

protected:
	std::string m_image_name;

	texture_description m_desc;

	uint8_t* m_data;

	ref<texture> m_texture;
};

}  // namespace setsuna

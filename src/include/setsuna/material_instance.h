#pragma once

#include <setsuna/color.h>
#include <setsuna/texture.h>
#include <setsuna/ref.h>
#include <string_view>
#include <vector>

/** @file
@brief Header for @ref setsuna::material_instance
*/

namespace setsuna {

/**
@brief Material instance

A material instance stores the data that required to render a specific material.

Material instances must be generated from a material, see @ref setsuna::material
for usage. Usually if two objects have the same material appearance, they could
share the same material instance. Modifying the shared material instance would affect
both objects simultaneously.

@see @ref setsuna::material
*/
class material_instance : public resource {

	RTTI_ENABLE(material_instance, resource);

	friend class material;

public:
	/**
	@brief Get scalar property

	Fetch the value of the property named @p name to @p out_value if it exists,
	otherwise @p out_value is untouched and the return value is false.
	*/
	bool get_property(std::string_view name, float& out_value);

	/**
	@brief Get color property

	Fetch the value of the property named @p name to @p out_value if it exists,
	otherwise @p out_value is untouched and the return value is false.
	*/
	bool get_property(std::string_view name, color& out_value);

	/**
	@brief Get texture property

	Fetch the value of the property named @p name to @p out_value if it exists,
	otherwise @p out_value is untouched and the return value is false.
	*/
	bool get_property(std::string_view name, setsuna::ref<texture>& out_value);

	/**
	@brief Set scalar property

	The function will do nothing if @p name is an undefined property or
	@p name is not a scalar property.
	*/
	void set_property(std::string_view name, float value);

	/**
	@brief Set color property

	The function will do nothing if @p name is an undefined property or
	@p name is not a color property.
	*/
	void set_property(std::string_view name, color value);

	/**
	@brief Set texture property

	The function will do nothing if @p name is an undefined property or
	@p name is not a texture property.
	*/
	void set_property(std::string_view name, setsuna::ref<texture> value);

private:
	// only called by material
	material_instance(const material& prototype);

	const material* m_prototype;

	std::vector<float> m_scalars;
	std::vector<color> m_colors;
	std::vector<setsuna::ref<texture>> m_textures;
};

}  // namespace setsuna

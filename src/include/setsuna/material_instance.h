#pragma once

#include <setsuna/color.h>
#include <setsuna/texture.h>
#include <setsuna/ref.h>
#include <string_view>
#include <vector>
#include <optional>

/** @file
@brief Header for @ref setsuna::material_instance
*/

namespace setsuna {

template<typename T>
using optional_reference = std::optional<std::reference_wrapper<T>>;

/**
@brief Material instance

A material instance stores the data that required to render a specific material.

Material instances must be generated from a material, see @ref setsuna::material
for usage. Usually if two objects have the same material appearance, they could
share the same material instance. Modifying the shared material instance would affect
both objects simultaneously.
*/
class material_instance : public resource {

	RTTI_ENABLE(material_instance, resource);

	friend class material;

	using reftex = setsuna::ref<texture>;

public:
	/**
	@brief Get or set the material property named @p name

	@tparam T Type of the property, has 1-to-1 correspondence with
	@ref setsuna::material::property_type

	@return An optional reference of the queried property

	For scalar property, @p T should be @p float;

	for color property, @p T should be @ref setsuna::color;

	for texture property, @p T should be @p ref<texture>.

	The @p name and the type @p T should match the definition of the material that
	generates this instance, otherwise the returned reference is invalid.
	*/
	template<typename T>
	optional_reference<T> property(std::string_view name) {
		auto search = std::find_if(m_prototype->properties_begin(),
		                           m_prototype->properties_end(),
		                           [&name](const auto& p) { return p.m_name == name; });
		if (search == m_prototype->properties_end()) {
			return std::nullopt;
		}

		if constexpr (std::is_same_v<T, float>) {
			if (search->m_type == material::SCALAR_PROPERTY) {
				return m_scalars[search->m_index];
			}
		}
		else if constexpr (std::is_same_v<T, color>) {
			if (search->m_type == material::COLOR_PROPERTY) {
				return m_colors[search->m_index];
			}
		}
		else if constexpr (std::is_same_v<T, reftex>) {
			if (search->m_type == material::TEXTURE_PROPERTY) {
				return m_textures[search->m_index];
			}
		}

		return std::nullopt;
	}

private:
	material_instance(const material& prototype) :
	    m_prototype{&prototype} {};

	const material* m_prototype;

	std::vector<float> m_scalars;
	std::vector<color> m_colors;
	std::vector<reftex> m_textures;
};

}  // namespace setsuna

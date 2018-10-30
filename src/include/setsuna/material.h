#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>

/** @file
@brief Header for @ref setsuna::material
*/

namespace setsuna {

class material_instance;

/**
@brief Material

A material serves as a prototype for the @ref setsuna::material_instance s.
Being a prototype means that a material only stores the information
of its properties rather than the actual values.

@section material-section-1 Define a material
To define a material, you need to specify what property of what kind the material
should have, for example

@code{.cpp}
material pbrmat;
pbrmat.define_properties(material::SCALAR_PROPERTY,  "roughness",
		                 material::SCALAR_PROPERTY,  "metallic",
		                 material::COLOR_PROPERTY,   "albedo",
		                 material::TEXTURE_PROPERTY, "normal map");
@endcode

defines a typical physically-based material.

Note the order and the names of properties matter because they have direct
correspondence to their counterpart in the shader.

@section material-section-2 Create a material instance

After defining a material, material instances which store different property
values can be generated from it. The following code creates a material instance
from previously defined material 'pbrmat' and sets its 'roughness' property to 0.5.

@code{.cpp}
auto mat_instance = pbrmat.create_instance();
mat_instance->set_property("roughness", 0.5);
@endcode

@attention The lifetime of a material must outlive the ones of instances that
generated from it.

@see @ref setsuna::material_instance
*/
class material {

public:
	/**
	@brief Type of material property
	*/
	enum property_type {
		SCALAR_PROPERTY, /**< @brief Scalar property */
		COLOR_PROPERTY,  /**< @brief Color property */
		TEXTURE_PROPERTY /**< @brief Texture property */
	};

private:
	struct property {

		property(property_type type, std::string_view& name, uint32_t index) :
		    m_type{type}, m_name(name), m_index{index} {}

		property_type m_type;
		std::string m_name;
		uint32_t m_index;
	};

public:
	/**
	@brief Default constructor
	*/
	material() = default;

	/**
	@brief Copying is not allowed
	*/
	material(const material&) = delete;

	material& operator=(const material&) = delete;

	/**
	@brief Define material properties

	See @ref material-section-1 for usage.

	One material does not allow two properties with the same name. Repeatedly
	defined properties will just be ignored.
	*/
	template<typename T, typename U, typename... Ts>
	void define_properties(T type, U name, Ts... remains) {
		add_property(type, name);
		if constexpr (sizeof...(remains) > 0) {
			define_properties(remains...);
		}
	}

	/**
	@brief Create a material instance

	See @ref material-section-2 for usage.
	*/
	ref<material_instance> create_instance() const;

	auto properties_begin() const noexcept { return m_properties.begin(); }
	auto properties_end() const noexcept { return m_properties.end(); }

private:
	void add_property(property_type type, std::string_view name);

private:
	std::vector<property> m_properties;

	std::unordered_map<property_type, uint32_t> m_counter;
};

}  // namespace setsuna

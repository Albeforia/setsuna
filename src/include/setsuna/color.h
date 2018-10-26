#pragma once

#include <glm/glm.hpp>

/** @file
@brief Header for @ref setsuna::color
*/

namespace setsuna {

/**
@brief RGBA color.

The color could be either in linear RGB space or sRGB space,
you are responsible to convert if necessary.
*/
struct color {

	/**
	@brief Construct a solid white color
	*/
	constexpr color() :
	    color(0xFFFFFF) {}

	/**
	@brief Construct a color from @p r , @p g , @p b , @p a values
	*/
	constexpr color(float r, float g, float b, float a) :
	    m_r{r}, m_g{g}, m_b{b}, m_a{a} {}

	/**
	@brief Construct an opaque color from @p r , @p g , @p b values
	*/
	constexpr color(float r, float g, float b) :
	    m_r{r}, m_g{g}, m_b{b}, m_a{1.0f} {}

	/**
	@brief Construct a color from a 24-bit(8-bit per channel) hexadecimal value @p hex
	and a transparency value @p a

	The @p hex value is treated as being in linear RGB space, i.e. every 8-bits is divided
	by 255 to get a normalized value. For example:

	@code{.cpp}
	constexpr color c(0xd2691e);	// c is {0.82353f, 0.41176f, 0.11765f, 1.0f}
	@endcode
	*/
	constexpr color(uint32_t hex, float a) :
	    m_r{(hex >> 16 & 255) / 255.0f},
	    m_g{(hex >> 8 & 255) / 255.0f},
	    m_b{(hex & 255) / 255.0f}, m_a{a} {}

	/**
	@brief Construct an opaque color from a 24-bit(8-bit per channel) hexadecimal value @p hex

	@see @ref color(uint32_t, float)
	*/
	constexpr explicit color(uint32_t hex) :
	    m_r{(hex >> 16 & 255) / 255.0f},
	    m_g{(hex >> 8 & 255) / 255.0f},
	    m_b{(hex & 255) / 255.0f}, m_a{1.0f} {}

	/**
	@brief Convert to a @p glm::vec3
	*/
	operator glm::vec3() const {
		return {m_r, m_g, m_b};
	}

	/**
	@brief Convert to a @p glm::vec4
	*/
	operator glm::vec4() const {
		return {m_r, m_g, m_b, m_a};
	}

private:
	float m_r, m_g, m_b, m_a;
};

}  // namespace setsuna

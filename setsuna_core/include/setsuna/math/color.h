#pragma once

#include <setsuna/math/vector.h>

/** @file
@brief Header for @ref setsuna::color
*/

namespace setsuna {

/**
@brief RGBA color.

Color in sRGB space cannot be directly used in calculations.
Use @ref setsuna::color::to_linear() to convert a color if it is sRGB.

@see @link setsuna::operator""_srgb() @endlink
*/
struct color {

	/**
	@brief Convert a color from sRGB to linear space
	*/
	static color to_linear(const color&);

	/**
	@brief Construct a solid white color
	*/
	constexpr color() :
	    color(1.0f) {}

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
	@brief Construct an opaque gray color
	*/
	constexpr color(float c) :
	    m_r{c}, m_g{c}, m_b{c}, m_a{1.0f} {}

	/**
	@brief Construct from a @ref setsuna::vector4
	*/
	color(vector4 v) {
		m_r = v.get_x();
		m_g = v.get_y();
		m_b = v.get_z();
		m_a = v.get_w();
	}

	/**
	@brief Convert to a @ref setsuna::vector4
	*/
	explicit operator vector4() const {
		return vector4(m_r, m_g, m_b, m_a);
	}

private:
	float m_r, m_g, m_b, m_a;
};

/**
@brief Create an opaque color from a 24-bit(8-bit per channel) hexadecimal value @p hex

We assume the provided hexadecimal value is sRGB.

@code{.cpp}
color c = 0xb22222_srgb; // {0.445201248, 0.0159962922, 0.0159962922, 1.0}	
@endcode
*/
color operator""_srgb(uint64_t hex);

}  // namespace setsuna

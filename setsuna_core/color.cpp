#include <setsuna/math/color.h>

using namespace DirectX;

namespace setsuna {

color color::to_linear(const color& c) {
	auto v = vector4(c);
	auto v1 = pow((v + vector4(0.055f)) * (1.0f / 1.055f), 2.4f);
	auto v2 = v * (1.0f / 12.92f);
	auto result = XMVectorSelect(v1,
	                             v2,
	                             XMVectorLess(v, XMVectorReplicate(0.04045f)));

	// keep alpha untouched
	return vector4(XMVectorSelect(v, result, g_XMSelect1110));
}

color operator""_srgb(uint64_t hex) {
	color c((hex >> 16 & 255) / 255.0f, (hex >> 8 & 255) / 255.0f, (hex & 255) / 255.0f);
	return color::to_linear(c);
}

}  // namespace setsuna

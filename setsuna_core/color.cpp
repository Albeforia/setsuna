#include <setsuna/color.h>

using namespace DirectX;

namespace setsuna {

color color::to_linear(const color& c) {
	auto v = XMVECTOR(c);
	XMVECTOR result = XMVectorPow(
	  XMVectorScale(
	    XMVectorAdd(v, XMVectorReplicate(0.055f)), 1.0f / 1.055f),
	  XMVectorReplicate(2.4f));
	result = XMVectorSelect(result,
	                        XMVectorScale(v, 1.0f / 12.92f),
	                        XMVectorLess(v, XMVectorReplicate(0.04045f)));

	return XMVectorSelect(v, result, g_XMSelect1110);
}

color operator""_srgb(std::size_t hex) {
	color c((hex >> 16 & 255) / 255.0f, (hex >> 8 & 255) / 255.0f, (hex & 255) / 255.0f);
	return color::to_linear(c);
}

}  // namespace setsuna

#include <setsuna/transform.h>

using namespace DirectX;

namespace setsuna {

transform::operator DirectX::XMMATRIX() const {
	XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);  // rotation origin
	return XMMatrixAffineTransformation(
	  XMLoadFloat3(&scale),
	  zero, XMLoadFloat4(&rotation),
	  XMLoadFloat3(&translation));
}

}  // namespace setsuna

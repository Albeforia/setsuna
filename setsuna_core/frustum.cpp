#include <setsuna/math/frustum.h>

namespace setsuna {

frustum::frustum(const matrix4& mat) {
	// ref: Fast Extraction of Viewing Frustum Planes from the WorldViewProjection Matrix

	float4x4 m(mat);

	planes[0] = plane(m(0, 3) + m(0, 0), m(1, 3) + m(1, 0), m(2, 3) + m(2, 0), m(3, 3) + m(3, 0));
	planes[1] = plane(m(0, 3) - m(0, 0), m(1, 3) - m(1, 0), m(2, 3) - m(2, 0), m(3, 3) - m(3, 0));
	planes[2] = plane(m(0, 3) + m(0, 1), m(1, 3) + m(1, 1), m(2, 3) + m(2, 1), m(3, 3) + m(3, 1));
	planes[3] = plane(m(0, 3) - m(0, 1), m(1, 3) - m(1, 1), m(2, 3) - m(2, 1), m(3, 3) - m(3, 1));
	planes[4] = plane(m(0, 2), m(1, 2), m(2, 2), m(3, 2));
	planes[5] = plane(m(0, 3) - m(0, 2), m(1, 3) - m(1, 2), m(2, 3) - m(2, 2), m(3, 3) - m(3, 2));
}

bool frustum::intersect(const aabb<3>& box) const {
	for (auto& plane : planes) {
		auto max_dist = box.min;
		if (plane.coeffs.x >= 0) {
			max_dist.x = box.max.x;
		}
		if (plane.coeffs.y >= 0) {
			max_dist.y = box.max.y;
		}
		if (plane.coeffs.z >= 0) {
			max_dist.z = box.max.z;
		}
		if (plane(max_dist) < 0) {
			return false;
		}
	}

	return true;
}

bool frustum::intersect(const sphere& sphere) const {
	for (auto& plane : planes) {
		if (plane(sphere.center) < -sphere.radius) {
			return false;
		}
	}

	return true;
}

}  // namespace setsuna

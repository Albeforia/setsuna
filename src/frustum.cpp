#include <setsuna/frustum.h>

namespace setsuna {

frustum::frustum(const glm::mat4& mat) {
	glm::vec3 xaxis(mat[0][0], mat[1][0], mat[2][0]);
	glm::vec3 yaxis(mat[0][1], mat[1][1], mat[2][1]);
	glm::vec3 zaxis(mat[0][2], mat[1][2], mat[2][2]);
	glm::vec3 waxis(mat[0][3], mat[1][3], mat[2][3]);

	planes[0] = plane(waxis + xaxis, mat[3][3] + mat[3][0]);
	planes[1] = plane(waxis - xaxis, mat[3][3] - mat[3][0]);
	planes[2] = plane(waxis - yaxis, mat[3][3] - mat[3][1]);
	planes[3] = plane(waxis + yaxis, mat[3][3] + mat[3][1]);
	planes[4] = plane(waxis + zaxis, mat[3][3] + mat[3][2]);
	planes[5] = plane(waxis - zaxis, mat[3][3] - mat[3][2]);
}

bool frustum::intersect(const aabb<3>& box) const {
	for (auto& plane : planes) {
		auto closest = box.min;
		if (plane.normal.x >= 0) {
			closest.x = box.max.x;
		}
		if (plane.normal.y >= 0) {
			closest.y = box.max.y;
		}
		if (plane.normal.z >= 0) {
			closest.z = box.max.z;
		}
		if (plane(closest) < 0) {
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

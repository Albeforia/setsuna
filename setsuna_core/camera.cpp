#include <setsuna/rtti_prefix.h>
#include <setsuna/camera.h>
#include <setsuna/object3d.h>

using namespace DirectX;

namespace setsuna {

camera::camera(object3d& o3d, type type,
               float fov_or_size, float aspect, float nearp, float farp) :
    component(o3d),
    m_type{type}, m_aspect{aspect}, m_near_plane{nearp}, m_far_plane{farp} {
	if (m_type == type::CT_PERSPECTIVE) {
		m_fov = fov_or_size;
		m_orthographic_size = 5.0;
	}
	else {
		m_fov = 60.0f;
		m_orthographic_size = fov_or_size;
	}

	update_projection();
}

void camera::update() {
	auto vmat = XMMatrixInverse(nullptr, m_object->world_matrix());
	auto pmat = XMLoadFloat4x4(&m_projection_matrix);
	XMStoreFloat4x4(&m_view_matrix, vmat);
	m_frustum = setsuna::frustum(XMMatrixMultiply(vmat, pmat));  // in world space
}

void camera::update_projection() {
	if (m_type == type::CT_PERSPECTIVE) {
		XMStoreFloat4x4(&m_projection_matrix,
		                XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fov), m_aspect, m_near_plane, m_far_plane));
	}
	else {
		float width = m_aspect * m_orthographic_size;
		XMStoreFloat4x4(&m_projection_matrix,
		                XMMatrixOrthographicLH(width * 2, m_orthographic_size * 2, m_near_plane, m_far_plane));
	}
}

void camera::set_aspect(float aspect) {
	m_aspect = aspect;
	update_projection();
}

}  // namespace setsuna

#include <setsuna/rtti_prefix.h>
#include <setsuna/object3d.h>
#include <glm/gtc/matrix_transform.hpp>

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
	m_view_matrix = glm::inverse(m_object->world_matrix());
	m_frustum = setsuna::frustum(m_projection_matrix * m_view_matrix);
}

void camera::update_projection() {
	if (m_type == type::CT_PERSPECTIVE) {
		m_projection_matrix = glm::perspective(
		  glm::radians(m_fov), m_aspect, m_near_plane, m_far_plane);
	}
	else {
		float width = m_aspect * m_orthographic_size;
		m_projection_matrix = glm::ortho(-width, width, -m_orthographic_size, m_orthographic_size,
		                                 m_near_plane, m_far_plane);
	}
}

void camera::set_aspect(float aspect) {
	m_aspect = aspect;
	update_projection();
}

}  // namespace setsuna

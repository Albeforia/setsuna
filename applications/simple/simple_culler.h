#pragma once

#include <setsuna/visitor.h>
#include <setsuna/ref.h>
#include <glm/glm.hpp>
#include <vector>

namespace setsuna {

class camera;
class mesh;
class material_instance;

}  // namespace setsuna

struct render_item {

	glm::mat4 world_matrix;
	setsuna::ref<setsuna::mesh> mesh;
	setsuna::ref<setsuna::material_instance> material;
};

class simple_culler : public setsuna::visitor {

public:
	enum class mode {
		CM_BOUNDING_BOX,
		CM_BOUNDING_SPHERE
	};

	simple_culler(setsuna::camera&, mode);

	void apply(setsuna::object3d&) override;

	std::vector<render_item> render_queue;

private:
	// do frustum culling according to this camera
	setsuna::camera* m_camera;

	mode m_cull_mode;
};

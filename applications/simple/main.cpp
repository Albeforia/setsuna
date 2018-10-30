#include <setsuna_app/app_glfw.h>
#include <setsuna/rtti_prefix.h>
#include <setsuna/object3d.h>
#include <setsuna/update_visitor.h>
#include <setsuna/shader_program.h>
#include <setsuna/geometry.h>
#include <setsuna/resource_manager.h>
#include <setsuna/texture_manager.h>
#include <setsuna_loaders/texture_loader.h>

#include <setsuna/material.h>
#include <setsuna/material_instance.h>
#include <setsuna/mesh_filter.h>
#include <setsuna/mesh_renderer.h>

#include "simple_culler.h"

using namespace setsuna;

class app : public app_glfw {

public:
	app(std::string_view name) :
	    app_glfw(name, 800, 600, 4, 5) {
		m_scene = new object3d();
	}

	~app() {
		delete m_scene;
		resource_manager::instance().shutdown();
	}

private:
	void init() override {
		// setup camera
		auto& camobj = m_scene->add_child();
		m_camera = &camobj.add_component<camera>(camera::type::CT_PERSPECTIVE, 75.0f,
		                                         m_window_width / float(m_window_height),
		                                         0.01f, 1000.0f);
		camobj.local_transform.translation.z = 5;

		// setup material
		m_material.define_properties(material::SCALAR_PROPERTY, "roughness",
		                             material::SCALAR_PROPERTY, "metalness",
		                             material::COLOR_PROPERTY, "tint",
		                             material::TEXTURE_PROPERTY, "albedo");
		auto mi = m_material.create_instance();

		// setup mesh
		auto mesh = geometry::sphere(2, 10, 10,
		                             -glm::pi<float>() * 1.4f, glm::pi<float>() * 0.65f);
		auto& meshobj = m_scene->add_child();
		meshobj.add_component<mesh_filter>(mesh);
		auto& renderer = meshobj.add_component<mesh_renderer>();
		renderer.material = mi;

		// setup texture
		//texture_manager::instance().set_option({false, 64});
		auto loader = resource_manager::instance().load<texture_loader>(
		  [mi](ref<resource> loaded) mutable {
			  mi->set_property("albedo", static_cast<texture*>(loaded.get()));
		  },
		  "textures/wood.jpg");
		mi->set_property("albedo", static_cast<texture*>(loader->get().get()));

		// setup shader
		m_shader_program.add_shader(shader_type::ST_VERTEX, "shaders/simple.vert");
		m_shader_program.add_shader(shader_type::ST_FRAGMENT, "shaders/simple.frag");
		m_shader_program.compile();
		m_shader_program.apply();
		m_shader_program.upload_uniform("projection", m_camera->projection_matrix());

		glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void update() override {
		resource_manager::instance().update();

		m_scene->accept(update_visitor{});
	}

	void render() override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader_program.upload_uniform("view",
		                                m_camera->view_matrix());

		simple_culler sc(*m_camera, simple_culler::mode::CM_BOUNDING_BOX);
		m_scene->accept(sc);

		for (auto& item : sc.render_queue) {
			m_shader_program.upload_uniform("world", item.world_matrix);
			ref<texture> tex;
			item.material->get_property("albedo", tex);
			m_shader_program.upload_uniform("u_tex",
			                                tex->address());

			item.mesh->render();
		}
	}

	void on_framebuffer_resize(int width, int height) override {
		app_glfw::on_framebuffer_resize(width, height);
		glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
		m_camera->set_aspect(m_window_width / float(m_window_height));
		m_shader_program.upload_uniform("projection", m_camera->projection_matrix());
	}

private:
	object3d* m_scene;
	camera* m_camera;
	material m_material;
	shader_program m_shader_program;
};

int main() {
	auto papp = new app("Simple");
	papp->start();
	delete papp;

	return 0;
}

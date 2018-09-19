#include <setsuna_app/app_glfw.h>
#include <setsuna/rtti_prefix.h>
#include <setsuna/object3d.h>
#include <setsuna/update_visitor.h>
#include <setsuna/shader_program.h>
#include <setsuna/geometry.h>

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

		// setup mesh
		m_test_mesh = geometry::sphere(2, 10, 10,
		                               -glm::pi<float>() * 1.4f, glm::pi<float>() * 0.65f);
		//m_test_mesh = geometry::plane(2, 2);

		// setup shader
		m_shader_program.add_shader(shader_type::ST_VERTEX, "shaders/simple.vert");
		m_shader_program.add_shader(shader_type::ST_FRAGMENT, "shaders/simple.frag");
		m_shader_program.compile();
		m_shader_program.apply();
		m_shader_program.upload_uniform("projection", m_camera->projection_matrix());

		glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void update() override {
		resource_manager::instance().update();

		m_scene->accept(update_visitor{});
	}

	void render() override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader_program.upload_uniform("world", glm::mat4(1.0));
		m_shader_program.upload_uniform("view",
		                                m_camera->view_matrix());

		m_test_mesh->render();
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

	ref<mesh> m_test_mesh;

	shader_program m_shader_program;
};

int main() {
	auto papp = new app("Simple");
	papp->start();
	delete papp;

	return 0;
}

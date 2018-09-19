// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <setsuna_app/app_glfw.h>
#include <iostream>

namespace setsuna {

app_glfw* app_glfw::_instance = nullptr;

app_glfw::app_glfw(std::string_view name,
                   uint32_t window_width, uint32_t window_height,
                   int gl_major, int gl_minor) :
    m_name(name),
    m_window_width{window_width}, m_window_height{window_height} {
	_instance = this;

	glfwSetErrorCallback(error_cb);

	if (!glfwInit()) {
		std::cerr << "Could not initialize GLFW" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	init_window(gl_major, gl_minor);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Could not initialize context" << std::endl;
		glfwDestroyWindow(m_window);
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
}

app_glfw::~app_glfw() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void app_glfw::init_window(int gl_major, int gl_minor) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(m_window_width, m_window_height,
	                            m_name.c_str(), nullptr, nullptr);
	if (!m_window) {
		std::cerr << "Could not create window";
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_window);

	// register callbacks
	glfwSetWindowSizeCallback(m_window, window_size_cb);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_cb);
	glfwSetCursorPosCallback(m_window, mouse_position_cb);
	glfwSetMouseButtonCallback(m_window, mouse_button_cb);
	glfwSetScrollCallback(m_window, mouse_scroll_cb);
	glfwSetKeyCallback(m_window, keyboard_cb);

	glfwGetFramebufferSize(m_window, &m_framebuffer_width, &m_framebuffer_height);

	// enable VSync
	glfwSwapInterval(1);
}

void app_glfw::start() {
	init();
	run();
}

void app_glfw::stop() {
	glfwSetWindowShouldClose(m_window, GL_TRUE);
}

void app_glfw::run() {
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
		update();
		render();
		glfwSwapBuffers(m_window);
	}
}

void app_glfw::on_window_resize(int width, int height) {
	m_window_width = width;
	m_window_height = height;
}

void app_glfw::on_framebuffer_resize(int width, int height) {
	m_framebuffer_width = width;
	m_framebuffer_height = height;
}

void app_glfw::error_cb(int, const char* description) {
	std::cerr << description << std::endl;
}

void app_glfw::window_size_cb(GLFWwindow*, int width, int height) {
	_instance->on_window_resize(width, height);
}

void app_glfw::framebuffer_size_cb(GLFWwindow*, int width, int height) {
	_instance->on_framebuffer_resize(width, height);
}

void app_glfw::mouse_position_cb(GLFWwindow*, double x, double y) {
	_instance->on_mouse_position(x, y);
}

void app_glfw::mouse_button_cb(GLFWwindow*, int button, int action, int mods) {
	_instance->on_mouse_button(button, action, mods);
}

void app_glfw::mouse_scroll_cb(GLFWwindow*, double xoffset, double yoffset) {
	_instance->on_mouse_scroll(xoffset, yoffset);
}

void app_glfw::keyboard_cb(GLFWwindow*, int key, int scancode, int action, int mods) {
	_instance->on_keyboard(key, scancode, action, mods);
}

}  // namespace setsuna

#pragma once

#include <string>
#include <string_view>

struct GLFWwindow;

namespace setsuna {

class app_glfw {

public:
	app_glfw(std::string_view name,
	         uint32_t window_width, uint32_t window_height,
	         int gl_major = 4, int gl_minor = 2);

	virtual ~app_glfw();

	// copying and moving are not allowed
	app_glfw(const app_glfw&) = delete;
	app_glfw(app_glfw&&) = delete;
	app_glfw& operator=(const app_glfw&) = delete;
	app_glfw& operator=(app_glfw&&) = delete;

	void start();
	void stop();

protected:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	// main loop
	void run();

	virtual void on_window_resize(int width, int height);
	// the size of a framebuffer may change independently of the size of a window
	virtual void on_framebuffer_resize(int width, int height);

	// input events
	virtual void on_mouse_position(double x, double y) {}
	virtual void on_mouse_button(int button, int action, int mods) {}
	virtual void on_mouse_scroll(double xoffset, double yoffset) {}
	virtual void on_keyboard(int key, int scancode, int action, int mods) {}

	std::string m_name;
	GLFWwindow* m_window;

	// DO NOT pass the window size to glViewport or other pixel-based OpenGL calls,
	// use the framebuffer size, which is in pixels, for pixel-based calls
	uint32_t m_window_width;
	uint32_t m_window_height;
	int m_framebuffer_width;
	int m_framebuffer_height;

	// used in glfw callbacks
	static app_glfw* _instance;

private:
	void init_window(int gl_major, int gl_minor);

	static void error_cb(int error, const char* description);
	static void window_size_cb(GLFWwindow*, int width, int height);
	static void framebuffer_size_cb(GLFWwindow*, int width, int height);
	static void mouse_position_cb(GLFWwindow*, double x, double y);
	static void mouse_button_cb(GLFWwindow*, int button, int action, int mods);
	static void mouse_scroll_cb(GLFWwindow*, double xoffset, double yoffset);
	static void keyboard_cb(GLFWwindow*, int key, int scancode, int action, int mods);
};

}  // namespace setsuna

#include "sys/window.hpp"

#include "sys/glfwcallbacks.hpp"


// =====================
// Construction/Destruction
// =====================

Window::Window(const v2i32& size_, const std::string& name, GLFWmonitor* monitor, GLFWwindow* share)
	: m_size{ size_ }
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(size_.x, size_.y, name.c_str(), monitor, share);

	if (!m_window)
		throw std::runtime_error("ERROR::WINDOW_CREATION::Window or context creation failed");


	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		throw std::runtime_error("ERROR::WINDOW_CREATION::Couldn't set glad proc address properly");

	glfwSetWindowUserPointer(m_window, this);

	glfwSetFramebufferSizeCallback(m_window, framebuffersize_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetWindowFocusCallback(m_window, window_focus_callback);


	glfwGetFramebufferSize(m_window, &m_size.x, &m_size.y);
	glViewport(0, 0, m_size.x, m_size.y); 
	
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	
	glfwSwapInterval(0);
}


Window::~Window() noexcept
{
	glfwDestroyWindow(m_window);
}


// =====================
// Actors
// =====================

void Window::clear_states() noexcept
{
	glfwPollEvents();
}


// =====================
// CallBacks
// =====================

bool Window::toggle_cursor() noexcept
{
	m_cursor_hidden = !m_cursor_hidden;

	glfwSetInputMode(m_window, GLFW_CURSOR, m_cursor_hidden ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	return m_cursor_hidden;
}


// =====================
// Setters
// =====================

void Window::resize(v2i32 new_size) noexcept
{
	glfwSetWindowSize(m_window, new_size.x, new_size.y);
	glViewport(0, 0, new_size.x, new_size.y);
	m_size = new_size;
}

v2f32 Window::to_gui_coordinates(const Window& window, v2f32 point) noexcept
{
	v2f32 mult1{ point - v2f32(window.m_size / 2) };
	v2f32 mult2{ g_gui_view_size.x / window.m_size.x,  g_gui_view_size.y / -window.m_size.y };

	return { mult1.x * mult2.x, mult1.y * mult2.y };
}
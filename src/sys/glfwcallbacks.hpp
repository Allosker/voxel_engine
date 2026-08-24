#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define basic gflw callbacks for event inputs
* ==============================================-
*/

#include "sys/graphics.hpp"
#include "sys/types.hpp"

#include "sys/window.hpp"


inline void framebuffersize_callback(GLFWwindow* window, int width, int height) noexcept
{
	auto* current_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
	v2i32 newsize = { width, height };

	current_window->add_event(Event::Resized{ newsize });
	current_window->resize(newsize);
}

inline void mouse_callback(GLFWwindow* window, double xpos, double ypos) noexcept
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->add_event(Event::MouseMoved{ { static_cast<float>(xpos), static_cast<float>(ypos) } });
}

inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) noexcept
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->add_event(Event::MouseWheelScrolled{ { static_cast<float>(xoffset), static_cast<float>(yoffset) } });
}

inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
	auto* current_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
		current_window->add_event(Event::KeyEvent{ static_cast<Keys>(key), Event::ButtonState::Press, static_cast<Modifiers>(mods) });
	else if (action == GLFW_RELEASE)
		current_window->add_event(Event::KeyEvent{ static_cast<Keys>(key), Event::ButtonState::Release, static_cast<Modifiers>(mods) });
}

inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) noexcept
{
	auto* current_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
		current_window->add_event(Event::MouseButtonEvent{ static_cast<MouseButtons>(button), Event::ButtonState::Press, static_cast<Modifiers>(mods) });
	else if (action == GLFW_RELEASE)
		current_window->add_event(Event::MouseButtonEvent{ static_cast<MouseButtons>(button), Event::ButtonState::Release, static_cast<Modifiers>(mods) });
}

inline void window_focus_callback(GLFWwindow* window, int focused) noexcept
{
	auto* current_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	current_window->add_event(Event::FocusChanged{ static_cast<bool>(focused) });
}
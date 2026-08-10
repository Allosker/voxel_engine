#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define basic gflw callbacks for event inputs
* ==============================================-
*/

#include "sys_graphics.hpp"
#include "sys_types.hpp"

#include "sys_window.hpp"


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
		current_window->add_event(Event::KeyPressed{ static_cast<Keys>(key), static_cast<KeyboardModes>(mods) });
	else if (action == GLFW_RELEASE)
		current_window->add_event(Event::KeyReleased{ static_cast<Keys>(key), static_cast<KeyboardModes>(mods) });
}

inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) noexcept
{
	auto* current_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
		current_window->add_event(Event::MouseButtonPressed{ static_cast<MouseButtons>(button), static_cast<KeyboardModes>(mods) });
	else if (action == GLFW_RELEASE)
		current_window->add_event(Event::MouseButtonReleased{ static_cast<MouseButtons>(button), static_cast<KeyboardModes>(mods) });
}
#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define Window object, responsible for a GUI window->on the user's OS.
* This file ensures that "event.hpp" be included with it;
* window-> are responsible for any keyboard/mouse/window->related events occurring when in focus.
* ==============================================-
*/

#include <string>
#include <queue>

#include "sys/graphics.hpp"
#include "sys/inputTypes.hpp"
#include "sys/event.hpp"



// GUI window->responsible for events related to it while in focus
// Note: cannot be copied/moved.
class Window
{
public:


	// = Construction/Initialization

	explicit Window(const v2i32& size_, const std::string& name, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

	Window(Window&) noexcept = delete;
	Window& operator=(Window&) noexcept = delete;

	Window(Window&&) noexcept = delete;
	Window& operator=(Window&&) noexcept = delete;

	~Window() noexcept;

	
	// = Actors

	void close() const noexcept { glfwSetWindowShouldClose(m_window, true); }

	void display() const noexcept { glfwSwapBuffers(m_window); }

	void clear_states() noexcept;


	bool toggle_cursor() noexcept;

	std::optional<Event> poll_event() noexcept
	{
		if(!m_queue_events.empty())
		{
			Event temp{ m_queue_events.front() };

			m_queue_events.pop();

			return std::make_optional(temp);
		}

		return std::nullopt;
	}
	
	template<typename EventSubType>
	void add_event(const EventSubType& type) noexcept
	{
		m_queue_events.push(type);
	}


	// = Getters

	GLFWwindow* get() noexcept { return m_window; }
	const GLFWwindow* const get() const noexcept { return m_window; }

	const mpml::Vector2<int>& getSize() const noexcept { return m_size; }


	// = Predicates

	bool isOpen() const noexcept { return !glfwWindowShouldClose(m_window); }

	bool isCursorHidden() const noexcept { return m_cursorHidden; }

	bool isKeyPressed(Keys key) const noexcept { return glfwGetKey(m_window, static_cast<int>(key)) == GLFW_PRESS; }
	bool isKeyReleased(Keys key) const noexcept { return glfwGetKey(m_window, static_cast<int>(key)) == GLFW_RELEASE; }

	bool isMouseButtonPressed(MouseButtons button) const noexcept { return glfwGetMouseButton(m_window, static_cast<int>(button)) == GLFW_PRESS; }
	bool isMouseButtonReleased(MouseButtons button) const noexcept { return glfwGetMouseButton(m_window, static_cast<int>(button)) == GLFW_RELEASE; }


	// = Setters

	void resize(const mpml::Vector2<int>& new_size) noexcept;


public:

	static v2f32 to_gui_coordinates(const Window& window, v2f32 point) noexcept;

	// Predefined size for the 2D GUI interface of the application
	static constexpr v2f32 g_gui_view_size{ 1920.f,1080.f };
		 

private:

	

	

	std::queue<Event> m_queue_events{};

	GLFWwindow* m_window{ nullptr };

	v2i32 m_size{};

	bool m_cursorHidden{ true };


};
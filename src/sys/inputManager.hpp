#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Singleton class that manages all inputs to redistribute them to any component
* ==============================================-
*/

#include <functional>
#include <queue>
#include <optional>

#include "sys/event.hpp"
#include "sys/inputTypes.hpp"

#include <print>

namespace sys
{

	class InputManager
	{
	public:

		using KeyFunc = std::function<void(Event::KeyEvent)>;
		using MouseFunc = std::function<void(Event::MouseButtonEvent)>;

		using KeyPair = std::pair<Event::KeyEvent, KeyFunc>;
		using MouseButtonPair = std::pair<Event::MouseButtonEvent, MouseFunc>;


		static InputManager& get() noexcept
		{
			static InputManager instance{};

			return instance;
		}


		void update()
		{
			update_states_and_callbacks<Event::KeyEvent, KeyPair>(m_key_funcs, m_key_events);
			update_states_and_callbacks<Event::MouseButtonEvent, MouseButtonPair>(m_mouseButton_funcs, m_mouseButton_events);
		}
		

		void add_key_event(Event::KeyEvent event) noexcept
		{
			m_key_events.push(event);
		}

		void add_mouseButton_event(Event::MouseButtonEvent event) noexcept
		{
			m_mouseButton_events.push(event);
		}
		

		void subscribe(const KeyFunc& func, Event::KeyEvent input) noexcept
		{
			m_key_funcs.emplace_back(input, func);
		}

		template<typename ClassType>
		void subscribe(void(ClassType::*Func)(Event::KeyEvent), ClassType& object, Event::KeyEvent input) noexcept
		{
			m_key_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}

		template<typename ClassType>
		void subscribe(void(ClassType::* Func)(Event::KeyEvent) const, const ClassType& object, Event::KeyEvent input) noexcept
		{
			m_key_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}


		void subscribe(const MouseFunc& func, Event::MouseButtonEvent input) noexcept
		{
			m_mouseButton_funcs.emplace_back(input, func);
		}

		template<typename ClassType>
		void subscribe(void(ClassType::* Func)(Event::MouseButtonEvent), ClassType& object, Event::MouseButtonEvent input) noexcept
		{
			m_mouseButton_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}

		template<typename ClassType>
		void subscribe(void(ClassType::* Func)(Event::MouseButtonEvent) const, const ClassType& object, Event::MouseButtonEvent input) noexcept
		{
			m_mouseButton_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}


		static bool pressed(Event::KeyEvent event, Keys scancode, Modifiers modes = {}) noexcept
		{
			return event.state == Event::ButtonState::Press && scancode == event.scancode && (static_cast<bool>(modes) ? static_cast<u8>(modes) & static_cast<u8>(event.modes) : true);
		}

		static bool released(Event::KeyEvent event, Keys scancode, Modifiers modes = {}) noexcept
		{
			return event.state == Event::ButtonState::Release && scancode == event.scancode && (static_cast<bool>(modes) ? static_cast<u8>(modes) & static_cast<u8>(event.modes) : true);
		}

		static bool pressed(Event::MouseButtonEvent event, MouseButtons scancode, Modifiers modes = {}) noexcept
		{
			return event.state == Event::ButtonState::Press && scancode == event.scancode && (static_cast<bool>(modes) ? static_cast<u8>(modes) & static_cast<u8>(event.modes) : true);
		}

		static bool released(Event::MouseButtonEvent event, MouseButtons scancode, Modifiers modes = {}) noexcept
		{
			return event.state == Event::ButtonState::Release && scancode == event.scancode && (static_cast<bool>(modes) ? static_cast<u8>(modes) & static_cast<u8>(event.modes) : true);
		}


	private:

		template<typename T, typename U>
		void update_states_and_callbacks(std::vector<U>& m_funcs, std::queue<T>& queue) noexcept
		{
			while (!queue.empty())
			{
				const auto event = queue.front();
				queue.pop();
			
				for (auto& i : m_funcs)
				{
					if (i.first.scancode == event.scancode)
					{
						i.first = event;
						i.second(i.first);
					}

				}
			}
		}


	private:

		InputManager() noexcept = default;

		std::vector<KeyPair> m_key_funcs{};
		std::vector<MouseButtonPair> m_mouseButton_funcs{};
		

		std::queue<Event::KeyEvent> m_key_events{};
		std::queue<Event::MouseButtonEvent> m_mouseButton_events{};

	};

}
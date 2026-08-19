#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Singleton class that manages all inputs to redistribute them to any component
* ==============================================-
*/

#include <functional>

#include "sys/event.hpp"
#include "sys/inputTypes.hpp"

namespace gfx
{

	class InputManager
	{
	public:

		struct KeyInput
		{
			Keys scancode;
			KeyboardModes modes;
		};

		struct MouseInput
		{
			MouseButtons scancode;
			KeyboardModes modes;
		};

		using KeyFunc = std::function<void(const KeyInput&)>;
		using MouseFunc = std::function<void(const MouseInput&)>;

		


		static InputManager& get() noexcept
		{
			static InputManager instance{};

			return instance;
		}

		void update_keys(const Event::KeyPressed& event)
		{
			for (auto& i : m_key_funcs)
			{
				if (i.first.scancode == event.scancode)
					i.second({ event.scancode, event.mode });
			}
		}
		
		void subscribe(const KeyFunc& func, const KeyInput& input) noexcept
		{
			m_key_funcs.emplace_back(input, func);
		}

		template<typename ClassType>
		void subscribe(void(ClassType::*Func)(const KeyInput&), ClassType& object, const KeyInput& input) noexcept
		{
			m_key_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}

		template<typename ClassType>
		void subscribe(void(ClassType::* Func)(const KeyInput&) const, const ClassType& object, const KeyInput& input) noexcept
		{
			m_key_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}


		void subscribe(const MouseFunc& func, const MouseInput& input) noexcept
		{
			m_mouse_funcs.emplace_back(input, func);
		}

		template<typename ClassType>
		void subscribe(void(ClassType::* Func)(const MouseInput&), ClassType& object, const MouseInput& input) noexcept
		{
			m_mouse_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}

		template<typename ClassType>
		void subscribe(void(ClassType::* Func)(const MouseInput&) const, const ClassType& object, const MouseInput& input) noexcept
		{
			m_mouse_funcs.emplace_back(input, std::bind(Func, &object, std::placeholders::_1));
		}



	private:

		InputManager() noexcept = default;

		std::vector<std::pair<KeyInput, KeyFunc>> m_key_funcs{};
		std::vector<std::pair<MouseInput, MouseFunc>> m_mouse_funcs{};

	};

}
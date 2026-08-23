#pragma once
/* -- All Rights Reserved: Allosker 2026
* https://github.com/Allosker/voxel_engine/blob/main/license.txt
* ==============================================-
*	Define an Event subsystem class that works in pair with Window
* ==============================================-
*/


#include <variant>

#include "sys/window.hpp"



class Event
{
public:

	enum class ButtonState : i8 { Press, Release };


	struct Resized
	{
		v2i32 size;
	};

	struct MouseButtonEvent
	{
		MouseButtons scancode;
		ButtonState state;
		Modifiers modes;
	};

	struct MouseMoved
	{
		v2f32 pos;
	};

	struct MouseWheelScrolled
	{
		v2f32 delta;
	};

	struct KeyEvent
	{
		Keys scancode;
		ButtonState state;
		Modifiers modes;
	};

	struct FocusChanged
	{
		bool focus;
	};

	
	// = Predicates

	template<typename EventSubType>
	bool is()
	{
		static_assert(isEventSubtype<EventSubType>, "EventSubType must be a subtype of the class Event");
		if constexpr (isEventSubtype<EventSubType>)
			return std::holds_alternative<EventSubType>(m_data);
	}

	template<typename EventSubType>
	const EventSubType* get_if()
	{
		static_assert(isEventSubtype<EventSubType>, "EventSubType must be a subtype of the class Event");
		if constexpr (isEventSubtype<EventSubType>)
			return std::get_if<EventSubType>(&m_data);
	}


	// = Initilisation

	template<typename EventSubType>
	Event(const EventSubType& type)
		: m_data{ std::in_place_type<EventSubType>, type }
	{
	}
	

private:

	std::variant <
		Resized,
		MouseButtonEvent,
		MouseMoved,
		MouseWheelScrolled,
		KeyEvent,
		FocusChanged
	> m_data;


	template <typename T, typename... Ts>
	[[nodiscard]] static constexpr bool isInVariant(const std::variant<Ts...>*)
	{
		return std::disjunction_v<std::is_same<T, Ts>...>;
	}

	template <typename T>
	static constexpr bool isEventSubtype = isInVariant<T>(decltype (&m_data)(nullptr));

};
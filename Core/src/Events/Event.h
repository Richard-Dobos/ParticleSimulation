#pragma once

#include <string>
#include <functional>

#include "Core.h"

#define EVENT_CLASS_CATEGORY(category)	virtual uint64_t getCategoryFlags() const override { return category; } 
#define EVENT_CLASS_TYPE(type)			static EventType getStaticType() { return EventType::##type; }\
										virtual EventType getEventType() const override { return getStaticType(); }\
										virtual const char* getName() const override { return #type; }

namespace Core::Event
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None						=	0,
		EventCategoryApplication	=	BIT(0),
		EventCategoryInput			=	BIT(1),
		EventCategoryKeyboard		=	BIT(2),
		EventCategoryMouse			=	BIT(3),
		EventCategoryMouseButton	=	BIT(4)
	};

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;

		virtual const char* getName() const = 0;
		virtual EventType getEventType() const = 0;
		virtual uint64_t getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); };

	public:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template <typename T>
		using EventFunction = std::function<bool>(T&);

	public:
		EventDispatcher(Event& event)
			:m_Event(event) {}

		template <typename T>
		bool dispatch(EventFunction<T> eventFunction)
		{
			if (m_Event.getEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = eventFunction(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& ostream, const Event& event) { return ostream << event.toString(); }
}
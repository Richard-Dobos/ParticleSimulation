#pragma once

#include <format>
#include <sstream>

#include "Event.h"

namespace Core::Event
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint16_t width, uint16_t height)
			:m_Width(width), m_Height(height) {}
			
		inline uint16_t getWidth() const { return m_Width; }
		inline uint16_t getHeight() const { return m_Height; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("WindowResizeEvent: [{} {}]", m_Width, m_Height);
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint16_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(int focused)
			:focused(focused) {}

		bool getFocused() const { return focused; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("WindowFocusEvent: {}", focused);
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		int focused;
	};
}

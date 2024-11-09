#pragma once

#include <format>
#include <sstream>

#include "Event.h"

namespace Core::Event
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double x, double y)
			:m_MouseX(x), m_MouseY(y) {}

		inline double getX() const { return m_MouseX; }
		inline double getY() const { return m_MouseY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("MouseMovedEvent: [{} {}]", m_MouseX, m_MouseY);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		double m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double xOffset, double yOffset)
			:m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline double getXOffset() const { return m_XOffset; }
		inline double getYOffset() const { return m_YOffset; }
		
		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("MouseScrolledEvent: [{} {}]", m_XOffset, m_YOffset);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);;

	private:
		double m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return m_Button; }
		
		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

	protected:
		MouseButtonEvent(int button)
			:m_Button(button) {}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("MouseButtonPressedEvent: {}", m_Button);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);

	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("MouseButtonReleasedEvent: {}", m_Button);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}
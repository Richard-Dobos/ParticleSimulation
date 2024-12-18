#pragma once

#include <format>
#include <sstream>

#include "Event.h"

namespace Core::Event
{
	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		KeyEvent(int keyCode)
			:m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			:KeyEvent(keyCode), m_RepeatCount(repeatCount) {
		};

		inline int getRepeatCount() const { return m_RepeatCount; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("KeyPressedEvent: {} ({}) repeats", m_KeyCode, m_RepeatCount);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	private:
		uint32_t m_RepeatCount;
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			:KeyEvent(keyCode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("KeyTypedEvent: {}", m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			:KeyEvent(keyCode) {
		}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << std::format("KeyReleasedEvent: {}", m_KeyCode);
			return ss.str();
		};
		
		EVENT_CLASS_TYPE(KeyReleased);
	};
}
#include "Event.h"

#include "glfw3.h"

namespace Core::Event
{
	inline void pollEvents()
	{
		glfwPollEvents();
	}

	void onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		//LOG_INFO("Event: {}", event.toString());
	}
}
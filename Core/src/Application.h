#include <string>
#include <vector>

#include "Core.h"
#include "Events/Event.h"

namespace Core
{
	class CORE_API Application
	{
	public:
		Application() = default;

		void Update();
		void OnEvent(Event::Event& event);
		void OnBegin();

	private:
		std::string findAssetFolder() const;
	};
}
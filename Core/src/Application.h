#include <iostream>
#include <string>

#include "Core.h"

#include "glew.h"
#include "glfw3.h"

namespace Core
{
	class CORE_API Application
	{
	public:
		Application() = default;

		void start();
		void Update();

	private:
		std::string findAssetFolder();
	};
}
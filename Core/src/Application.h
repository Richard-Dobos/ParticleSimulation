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

		virtual void start();
		void Update();
		
	private:
		void runTime();

	private:
		std::string findAssetFolder() const;
	};
}
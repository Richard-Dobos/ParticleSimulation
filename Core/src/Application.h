#include <iostream>
#include <string>

#include "Core.h"

namespace Core
{
	class CORE_API Application
	{
	public:
		Application() = default;

		void Update();
		void OnBegin();

	private:
		void Runtime();
		
		std::string findAssetFolder() const;
	};
}
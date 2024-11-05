#include <iostream>
#include <string>
#include <vector>

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
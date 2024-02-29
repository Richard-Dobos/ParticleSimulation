#pragma once

#include "Components/Component.h"

namespace Core::Engine
{
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		template<typename T>
		T getComponent() { return T; }

		uint64_t GetUID() { return UID; }

	private:
		uint64_t UID;
	};
}
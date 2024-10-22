#pragma once

#include <map>
#include<iostream>

#include "Components/Components.h"

namespace Core::Engine
{
	class GameObject
	{
	public:
		GameObject() = default;
		~GameObject() = default;

	public:
		uint64_t UID;
	};
}
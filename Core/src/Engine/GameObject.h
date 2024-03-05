#pragma once

#include "map"

#include "Components/Components.h"

namespace Core::Engine
{
	class GameObject
	{
	public:
		GameObject(Transform transform)
			:m_Transform(transform) {}
		GameObject() = default;
		~GameObject() = default;

	public:
		Material m_Material;
		Transform m_Transform;
	};
}
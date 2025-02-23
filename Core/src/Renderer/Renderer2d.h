#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Core.h"

#include "glew.h"
#include "glm.hpp"
#include "Buffer.h"
#include "VertexArray.h"
#include "Utils/Shader.h"
#include "Engine/Components/Components.h"

namespace Core::Renderer
{
	class CORE_API Renderer2d
	{
	public:
		Renderer2d();
		~Renderer2d();

		void beginBatch();
		void endBatch();
		void flush() const;

		void DrawQuad(const Engine::Components::Transform& transform, const Engine::Components::Color& color);
		void DrawQuads(const Engine::Components::Transform* transform, const Engine::Components::Color* color, uint32_t numberOfQuads);

	public:
		uint16_t m_DrawCalls = 0;

	private:
		BufferLayout m_BufferLayout;
	};

	class InstancedRenderer2d
	{
	public:
		InstancedRenderer2d();
		~InstancedRenderer2d();

	private:
		BufferLayout m_BufferLayout;
	};
}
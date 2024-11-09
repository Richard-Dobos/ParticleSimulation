#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Core.h"

#include "glm.hpp"
#include "glew.h"
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

		void DrawQuad(const Engine::Transform& transform, const Engine::Color& color);

	public:
		uint16_t drawCalls = 0;

	private:
		BufferLayout m_BufferLayout;
	};

	class CORE_API InstancedRenderer2d
	{
	public:
		InstancedRenderer2d();
		~InstancedRenderer2d();

	private:
		BufferLayout m_BufferLayout;
	};
}
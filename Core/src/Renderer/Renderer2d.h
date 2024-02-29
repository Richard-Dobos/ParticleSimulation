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

namespace Core::Renderer
{
	class Renderer2d
	{
	public:
		Renderer2d();
		~Renderer2d();

		void beginBatch();
		void endBatch();
		void flush() const;

		void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) const;
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;
	};
}
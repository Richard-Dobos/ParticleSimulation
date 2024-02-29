#pragma once

#include <vector>
#include <memory>

#include "Buffer.h"
#include "Utils/Shader.h"
#include "glew.h"
#include "glm.hpp"

namespace Core::Renderer
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind() const;
		void unBind() const;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		std::shared_ptr<IndexBuffer> getIndexBuffer() const { return m_IndexBuffer; }
		std::shared_ptr<VertexBuffer> getVertexBuffer() const { return m_VertexBuffer; }
	private:
		uint32_t m_VertexArrayID;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
	};
}
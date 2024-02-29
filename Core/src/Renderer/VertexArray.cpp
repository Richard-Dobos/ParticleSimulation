#include "VertexArray.h"

namespace Core::Renderer
{
	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void VertexArray::unBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		m_VertexBuffer = vertexBuffer;
		vertexBuffer->bind();

		BufferLayout layout = vertexBuffer->getBufferLayout();
		uint32_t index = 0;

		if (layout.begin() == layout.end())
			std::cout << YELLOW << "Buffer Layout not found!\n" << RESET;

		else
		{
			for (const auto& element : layout)
			{
				glVertexAttribPointer(index, element.ElementCount, 
					ShaderDataTypeToOpenGLDataType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, 
					layout.getStride(), (const void*)element.Offset);
				glEnableVertexAttribArray(index);
				index += 1;
			}
		}
	}

	void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;
	}
}
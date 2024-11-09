#pragma once

#include <iostream>
#include <vector>

#include "glew.h"
#include "glm.hpp"

namespace Core::Renderer
{
	enum class ShaderDataType : uint8_t
	{
		None = 0, Float1, Float2, Float3, Float4, uInt1, uInt2, uInt3, uInt4, Int1, Int2, Int3, Int4, Mat3, Mat4
	};

	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType Type)
	{
		switch (Type)
		{
		case ShaderDataType::Float1:	return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Int1:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::uInt1:		return GL_UNSIGNED_INT;
		case ShaderDataType::uInt2:		return GL_UNSIGNED_INT;
		case ShaderDataType::uInt3:		return GL_UNSIGNED_INT;
		case ShaderDataType::uInt4:		return GL_UNSIGNED_INT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		default:						return 0;
		}
	}

	static uint32_t ShaderDataTypeSize(ShaderDataType Type)
	{
		switch (Type)
		{
		case ShaderDataType::Float1:	return 4 * 1;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Int1:		return 4 * 1;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::uInt1:		return 4 * 1;
		case ShaderDataType::uInt2:		return 4 * 2;
		case ShaderDataType::uInt3:		return 4 * 3;
		case ShaderDataType::uInt4:		return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 9;
		case ShaderDataType::Mat4:		return 4 * 16;
		default:						return 0;
		}
	}
	
	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		uint32_t ElementCount;
		bool Normalized;

		BufferElement(const std::string& name, ShaderDataType type, bool normalized)
			:Name(name), Type(type), Size(ShaderDataTypeSize(Type)), ElementCount(GetElementCount(type)), Normalized(normalized) {}

	private:
		uint32_t GetElementCount(ShaderDataType Type) const
		{
			switch (Type)
			{
			case ShaderDataType::Float1:	return 1; 
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Int1:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::uInt1:		return 1;
			case ShaderDataType::uInt2:		return 2;
			case ShaderDataType::uInt3:		return 3;
			case ShaderDataType::uInt4:		return 4;
			case ShaderDataType::Mat3:		return 9;
			case ShaderDataType::Mat4:		return 16;
			default:						return 0;
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& BufferElements)
			:m_BufferElements(BufferElements)
		{
			calculateOffsetAndStride();
		}

		uint32_t getStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }

	private:
		void calculateOffsetAndStride()
		{
			for(auto& element : m_BufferElements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_BufferElements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(const std::vector<float>& verticies);
		VertexBuffer(uint32_t size);
		~VertexBuffer();

		void bind() const;
		void unBind() const;
		void setData(const void* bufferData, uint32_t size) const;

		void setBufferLayout(const BufferLayout& bufferLayout) { m_BufferLayout = bufferLayout; }
		const BufferLayout& getBufferLayout() const { return m_BufferLayout; }
		uint32_t getVertexBuffer() const { return m_VertexBufferID; }

	private:
		uint32_t m_VertexBufferID;
		BufferLayout m_BufferLayout;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const std::vector<uint32_t>& indecies);
		IndexBuffer(const uint32_t* indecies, uint32_t count);
		~IndexBuffer();

		void bind() const;
		void unBind() const;

		uint32_t getIndeciesCount() const { return m_IndeciesCount; }
		uint32_t getIndexBuffer() const { return m_IndexBufferID; }

	private:
		uint32_t m_IndeciesCount;
		uint32_t m_IndexBufferID;
	};
}
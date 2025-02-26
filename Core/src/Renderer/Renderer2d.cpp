#include "Renderer2d.h"

#include "glew.h"
#include "gtc/matrix_transform.hpp"

namespace Core::Renderer
{
	struct Vertex
	{
		glm::vec3 Pos;	
		uint32_t Color;
	};

	struct RendererStorage
	{
		const uint32_t QuadBatchSize = 50000;
		const uint32_t VertexCountPerBatch = QuadBatchSize * 4;
		const uint32_t IndexCountPerBatch = QuadBatchSize * 6;

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;

		uint32_t IndexCount = 0;
		Vertex* QuadVertexBasePtr = nullptr;
		Vertex* QuadVertexPtr = nullptr;
	};

	static RendererStorage s_Data;

	Renderer2d::Renderer2d()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_BufferLayout =
		{
			{"Pos", ShaderDataType::Float3, false},
			{"Color", ShaderDataType::Float1, false}
		};
		
		s_Data.QuadVertexArray = std::make_shared<VertexArray>();
		s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(sizeof(Vertex) * s_Data.VertexCountPerBatch);

		s_Data.QuadVertexBasePtr = new Vertex[s_Data.VertexCountPerBatch * sizeof(Vertex)];

		s_Data.QuadVertexBuffer->setBufferLayout(m_BufferLayout);
		s_Data.QuadVertexArray->addVertexBuffer(s_Data.QuadVertexBuffer);
		
		uint32_t* quadIndecies = new uint32_t[s_Data.IndexCountPerBatch];
		uint32_t offset = 0;

		for(uint32_t i = 0; i < s_Data.IndexCountPerBatch; i+=6)
		{
			quadIndecies[i] = offset;
			quadIndecies[i + 1] = offset + 1;
			quadIndecies[i + 2] = offset + 2;

			quadIndecies[i + 3] = offset + 2;
			quadIndecies[i + 4] = offset + 3;
			quadIndecies[i + 5] = offset;

			offset += 4;
		}

		auto QuadIBO = std::make_shared<IndexBuffer>(quadIndecies, s_Data.IndexCountPerBatch);
		s_Data.QuadVertexArray->setIndexBuffer(QuadIBO);

		delete[] quadIndecies;
	}

	Renderer2d::~Renderer2d()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glDisable(GL_DEPTH_TEST);
	}

	void Renderer2d::endBatch()
	{
		flush();
	}

	void Renderer2d::beginBatch()
	{
		s_Data.QuadVertexPtr = s_Data.QuadVertexBasePtr;
		s_Data.IndexCount = 0;

		m_DrawCalls += 1;
	}

	void Renderer2d::DrawQuad(const Engine::Components::Transform& transform, const Engine::Components::Color& color)
	{
		if (s_Data.IndexCount >= s_Data.IndexCountPerBatch)
		{
			endBatch();
			beginBatch();
		}		

		uint32_t packedColor = color.r << 24 | color.g << 16 | color.b << 8 | color.a;

		// Bottom Left Corner Vertex
		s_Data.QuadVertexPtr->Pos = transform.pos;
		s_Data.QuadVertexPtr->Color = packedColor;
		s_Data.QuadVertexPtr++;

		// Bottom Right Corner Vertex
		s_Data.QuadVertexPtr->Pos = { transform.pos.x + transform.scale.x, transform.pos.y, transform.scale.z };
		s_Data.QuadVertexPtr->Color = packedColor;
		s_Data.QuadVertexPtr++;

		// Top Right Corner Vertex
		s_Data.QuadVertexPtr->Pos = { transform.pos.x + transform.scale.x, transform.pos.y + transform.scale.y, transform.pos.z };
		s_Data.QuadVertexPtr->Color = packedColor;
		s_Data.QuadVertexPtr++;

		//Top Left Corner Vertex
		s_Data.QuadVertexPtr->Pos = { transform.pos.x, transform.pos.y + transform.scale.y, transform.pos.z };
		s_Data.QuadVertexPtr->Color = packedColor;
		s_Data.QuadVertexPtr++;

		s_Data.IndexCount += 6;
	}

	void Renderer2d::DrawQuads(const Engine::Components::Transform* transform, const Engine::Components::Color* color, uint32_t numberOfQuads)
	{
		m_DrawCalls += 1;
		
		for (uint32_t i = 0; i < numberOfQuads; i++)
		{
			if (s_Data.IndexCount >= s_Data.IndexCountPerBatch)
			{
				m_DrawCalls += 1;
				endBatch();
				beginBatch();
			}

			uint32_t packedColor = color[i].r << 24 | color[i].g << 16 | color[i].b << 8 | color[i].a;

			// Bottom Left Corner Vertex
			s_Data.QuadVertexPtr->Pos = transform[i].pos;
			s_Data.QuadVertexPtr->Color = packedColor;
			s_Data.QuadVertexPtr++;

			// Bottom Right Corner Vertex
			s_Data.QuadVertexPtr->Pos = { transform[i].pos.x + transform[i].scale.x, transform[i].pos.y, transform[i].scale.z };
			s_Data.QuadVertexPtr->Color = packedColor;
			s_Data.QuadVertexPtr++;

			// Top Right Corner Vertex
			s_Data.QuadVertexPtr->Pos = { transform[i].pos.x + transform[i].scale.x, transform[i].pos.y + transform[i].scale.y, transform[i].pos.z };
			s_Data.QuadVertexPtr->Color = packedColor;
			s_Data.QuadVertexPtr++;

			//Top Left Corner Vertex
			s_Data.QuadVertexPtr->Pos = { transform[i].pos.x, transform[i].pos.y + transform[i].scale.y, transform[i].pos.z };
			s_Data.QuadVertexPtr->Color = packedColor;
			s_Data.QuadVertexPtr++;

			s_Data.IndexCount += 6;
		}
	}

	void Renderer2d::flush() const 
	{
		uint32_t size = (uint8_t*)s_Data.QuadVertexPtr - (uint8_t*)s_Data.QuadVertexBasePtr;

		s_Data.QuadVertexBuffer->setData(s_Data.QuadVertexBasePtr, size);

		glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, NULL);
	}
}
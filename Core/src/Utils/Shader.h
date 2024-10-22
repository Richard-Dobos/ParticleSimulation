#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

#include "Core.h"
#include "glew.h"

namespace Core::Utils
{
	enum CORE_API ShaderUniformDataType : uint8_t
	{
		SHADER_UNIFORM_FLOAT = 0,
		SHADER_UNIFORM_VEC2,
		SHADER_UNIFROM_VEC3,
		SHADER_UNIFORM_VEC4,
		SHADER_UNIFORM_INT,
		SHADER_UNIFORM_IVEC2,
		SHADER_UNIFROM_IVEC3,
		SHADER_UNIFORM_IVEC4,
		SHADER_UNIFORM_SAMPLER2D,
		SHADER_UNIFORM_MAT2X2,
		SHADER_UNIFROM_MAT3X3,
		SHADER_UNIFROM_MAT4X4
	};

	class CORE_API Shader
	{
	public:
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
		~Shader();

		void bind() const;
		void unBind() const;

		void setUniformVector(const std::string& name, ShaderUniformDataType uniformDataType, const void* value, uint32_t count);

	private:
		GLint getUniformLocation(const std::string& name);

		std::string parseShader(const char* shaderPath) const;

		bool createShaderProgram(const char* shaderSource, GLenum shaderType) const;
		bool checkShaderCompilationStatus(uint32_t shaderID) const;
		void checkShaderErrorMessage(uint32_t shaderID, GLenum shaderType) const;

	private:
		std::unordered_map<std::string, GLint> m_UniformCache;

		std::string m_ComputeShaderPath;
		std::string m_VertexShaderSource;
		std::string m_FragmentShaderSource;
		uint32_t m_ShaderProgramID;
	};
};
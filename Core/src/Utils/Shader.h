#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

#include "Core.h"
#include "glew.h"
#include "glm.hpp"

namespace Core::Utils
{
	class Shader
	{
	public:
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		~Shader();

		void bind() const;
		void unBind() const;
		

		void setUniform1f(const std::string& name, float uniform);
		void setUniform2f(const std::string& name, glm::vec2 uniform);
		void setUniform3f(const std::string& name, glm::vec3 uniform);
		void setUniform4f(const std::string& name, glm::vec4 uniform);

		void setUniform1i(const std::string& name, int uniform);
		void setUniform2i(const std::string& name, glm::ivec2 uniform);
		void setUniform3i(const std::string& name, glm::ivec3 uniform);
		void setUniform4i(const std::string& name, glm::ivec4 uniform);

		void setUniformMat3f(const std::string& name,const glm::mat3& uniform);
		void setUniformMat4f(const std::string& name, const glm::mat4& uniform);

	private:
		GLint getUniformLocation(const std::string& name);

		std::string parseShader(const char* shaderPath) const;
		
		bool createShaderProgram(const char* shaderSource, GLenum shaderType) const;
		bool checkShaderCompilationStatus(uint32_t shaderID) const;
		void checkShaderErrorMessage(uint32_t shaderID, GLenum shaderType) const;

	private:
		std::unordered_map<std::string, GLint> m_UniformCache;

		std::string m_VertexShaderSource;
		std::string m_FragmentShaderSource;
		uint32_t m_ShaderProgramID;
	};
}

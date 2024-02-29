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
		
		GLint getUniformLocation(const std::string& name);

		void setUniform1f(GLint location, float uniform) const;
		void setUniform2f(GLint location, glm::vec2 uniform) const;
		void setUniform3f(GLint location, glm::vec3 uniform) const;
		void setUniform4f(GLint location, glm::vec4 uniform) const;

		void setUniform1i(GLint location, int uniform) const;
		void setUniform2i(GLint location, glm::ivec2 uniform) const;
		void setUniform3i(GLint location, glm::ivec3 uniform) const;
		void setUniform4i(GLint location, glm::ivec4 uniform) const;

		void setUniformMat3f(GLint location,const glm::mat3& uniform) const;
		void setUniformMat4f(GLint location,const glm::mat4& uniform) const;

	private:
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

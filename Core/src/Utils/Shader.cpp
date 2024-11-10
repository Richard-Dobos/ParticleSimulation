#include <fstream>
#include <sstream>
#include <cstdarg>

#include "Shader.h"
#include "glm.hpp"

namespace Core::Utils
{
	Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
	{
		m_ShaderProgramID = glCreateProgram();

		LOG_TRACE("Compiling Shader {}: {}", m_ShaderProgramID, vertexShaderFilePath);
		LOG_TRACE("Compiling Shader {}: {}", m_ShaderProgramID, fragmentShaderFilePath);

		m_VertexShaderSource = parseShader(vertexShaderFilePath);
		m_FragmentShaderSource = parseShader(fragmentShaderFilePath);

		if (createShaderProgram(m_VertexShaderSource.c_str(), GL_VERTEX_SHADER) && createShaderProgram(m_FragmentShaderSource.c_str(), GL_FRAGMENT_SHADER))
			glLinkProgram(m_ShaderProgramID);

		else
			glDeleteProgram(m_ShaderProgramID);
	}

	Shader::Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
	{
		m_ShaderProgramID = glCreateProgram();

		LOG_TRACE("Compiling Shaders {}", m_ShaderProgramID);

		if (createShaderProgram(vertexShaderCode.c_str(), GL_VERTEX_SHADER) && createShaderProgram(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER))
			glLinkProgram(m_ShaderProgramID);

		else
			glDeleteProgram(m_ShaderProgramID);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderProgramID);
	}

	std::string Shader::parseShader(const char* shaderPath) const
	{
		std::ifstream stream(shaderPath);
		std::stringstream shaderSource;

		if (!stream.is_open())
		{
			if (shaderPath == nullptr)
				LOG_ERROR("Shader Path Not Specified.");
			else
				LOG_ERROR("Failed to open SHADER at path : {}", shaderPath);

			return "\0";
		}

		std::string line;

		while (std::getline(stream, line))
		{
			shaderSource << line << "\n";
		}

		LOG_INFO("Shader Parse Successful.");

		return shaderSource.str();
	}

	bool Shader::createShaderProgram(const char* shaderSource, GLenum shaderType) const
	{
		uint32_t shaderID = glCreateShader(shaderType);

		glShaderSource(shaderID, 1, &shaderSource, nullptr);
		glCompileShader(shaderID);

		if (!checkShaderCompilationStatus(shaderID))
		{
			checkShaderErrorMessage(shaderID, shaderType);
			return false;
		}

		glAttachShader(m_ShaderProgramID, shaderID);
		return true;
	}

	bool Shader::checkShaderCompilationStatus(uint32_t shaderID) const
	{
		GLint compileStatus;

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus == GL_FALSE)
			return false;

		return true;
	}

	void Shader::checkShaderErrorMessage(uint32_t shaderID, GLenum shaderType) const
	{
		GLint logLength = 0;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> messageBuffer(logLength);
		glGetShaderInfoLog(shaderID, logLength, nullptr, messageBuffer.data());

		switch (shaderType)
		{
		default:
			LOG_ERROR("Shader compilation failed!\n{}", messageBuffer.data());
			break;

		case GL_VERTEX_SHADER:
			LOG_ERROR("Vertex Shader compilation failed!\n{}", messageBuffer.data());
			break;

		case GL_FRAGMENT_SHADER:
			LOG_ERROR("Fragment Shader compilation failed!\n{}", messageBuffer.data());
			break;

		case GL_GEOMETRY_SHADER:
			LOG_ERROR("Geometry Shader compilation failed!\n{}", messageBuffer.data());
			break;

		case GL_TESS_EVALUATION_SHADER:
			LOG_ERROR("Tessellation Evaluation Shader compilation failed!\n{}", messageBuffer.data());
			break;

		case GL_TESS_CONTROL_SHADER:
			LOG_ERROR("Tessellation Control Shader compilation failed!\n{}", messageBuffer.data());
			break;
		}
	}

	GLint Shader::getUniformLocation(const std::string& name)
	{
		if (m_UniformCache.contains(name))
			return m_UniformCache[name];

		GLint location = glGetUniformLocation(m_ShaderProgramID, name.c_str());

		if (location == -1)
			LOG_ERROR("Uniform {} doesn't exist.", name);

		else
			m_UniformCache[name] = location;

		return location;
	}

	void bind(uint32_t shaderID)
	{
		glUseProgram(shaderID);
	}

	void unBind()
	{
		glUseProgram(0);
	}

	uint32_t getCurrentShaderID()
	{
		int currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

		return currentProgram;
	}

	void Shader::setUniformVector(const std::string& name, ShaderUniformDataType uniformDataType, const void* value, uint32_t count)
	{
		switch (uniformDataType)
		{
		case ShaderUniformDataType::SHADER_UNIFORM_FLOAT:
			glUniform1fv(getUniformLocation(name), count, (const GLfloat*) value);
			break;

		case ShaderUniformDataType::SHADER_UNIFORM_VEC2:
			glUniform2fv(getUniformLocation(name), count, (const GLfloat*) value);
			break;

		case ShaderUniformDataType::SHADER_UNIFROM_VEC3:
			glUniform3fv(getUniformLocation(name), count, (const GLfloat*)value);
			break;

		case ShaderUniformDataType::SHADER_UNIFORM_VEC4:
			glUniform4fv(getUniformLocation(name), count, (const GLfloat*)value);
			break;

		case ShaderUniformDataType::SHADER_UNIFORM_INT:
			glUniform1iv(getUniformLocation(name), count, (const GLint*)value);
			break;

		case ShaderUniformDataType::SHADER_UNIFORM_IVEC2:
			glUniform2iv(getUniformLocation(name), count, (const GLint*)value);
			break;
			
		case ShaderUniformDataType::SHADER_UNIFROM_IVEC3:
			glUniform3iv(getUniformLocation(name), count, (const GLint*)value);
			break;

		case ShaderUniformDataType::SHADER_UNIFORM_IVEC4:
			glUniform4iv(getUniformLocation(name), count, (const GLint*)value);
			break;

		default:
			LOG_ERROR("Uniform Data Type doesn't exist!");
		}
	}

	void Shader::setUniformMat4x4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	uint32_t Shader::getShaderProgramID() const
	{
		return m_ShaderProgramID;
	}
}
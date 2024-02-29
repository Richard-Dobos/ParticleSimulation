#include <fstream>
#include <sstream>
#include <cstdarg>

#include "Shader.h"

namespace Core::Utils
{
	Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		std::cout << YELLOW_BACKGROUND << BLACK << "Compiling Shader: " << vertexShaderPath << '\n' << RESET;
		std::cout << YELLOW_BACKGROUND << BLACK << "Compiling Shader: " << fragmentShaderPath << '\n' << RESET;

		m_ShaderProgramID = glCreateProgram();
		m_VertexShaderSource = parseShader(vertexShaderPath);
		m_FragmentShaderSource = parseShader(fragmentShaderPath);

		if (createShaderProgram(m_VertexShaderSource.c_str(), GL_VERTEX_SHADER) && createShaderProgram(m_FragmentShaderSource.c_str(), GL_FRAGMENT_SHADER))
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
			shaderPath == nullptr ? std::cout << GREEN_BACKGROUND<< BLACK << "Shader Path Not Specified.\n" << RESET : std::cout << RED_BACKGROUND << BLACK<< "ERROR: Failed to open SHADER at path: " << shaderPath << "\n" << RESET;
			return "\0";
		}

		std::string line;

		while (std::getline(stream, line))
		{
			shaderSource << line << "\n";
		}

		std::cout << GREEN_BACKGROUND << BLACK << "Shader Parse Successful.\n" << RESET;

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
			std::cout << RED_BACKGROUND << BLACK << "Shader compilation failed!\n" << messageBuffer.data() << RESET;
			break;

		case GL_VERTEX_SHADER:
			std::cout << RED_BACKGROUND << BLACK << "Vertex Shader compilation failed!\n" << messageBuffer.data() << RESET;
			break;

		case GL_FRAGMENT_SHADER:
			std::cout << RED_BACKGROUND << BLACK << "Fragment Shader compilation failed!\n" << messageBuffer.data() << RESET;
			break;

		case GL_GEOMETRY_SHADER:
			std::cout << RED_BACKGROUND << BLACK << "Geometry Shader compilation failed!\n" << messageBuffer.data() << RESET;
			break;

		case GL_TESS_EVALUATION_SHADER:
			std::cout << RED_BACKGROUND << BLACK << "Tessellation Evaluation Shader compilation failed!\n" << messageBuffer.data() << RESET;
			break;

		case GL_TESS_CONTROL_SHADER:
			std::cout << RED_BACKGROUND << BLACK << "Tessellation Control Shader compilation failed!\n" << messageBuffer.data() << RESET;
			break;
		}
	}

	GLint Shader::getUniformLocation(const std::string& name)
	{
		if (m_UniformCache.contains(name))
			return m_UniformCache[name];

		GLint location = glGetUniformLocation(m_ShaderProgramID, name.c_str());

		if (location == -1)
			std::cout << RED_BACKGROUND << BLACK << "Uniform " << name << " doesn't exist.\n" << RESET;

		else
			m_UniformCache[name] = location;

		return location;
	}

	void Shader::bind() const
	{
		glUseProgram(m_ShaderProgramID);
	}

	void Shader::unBind() const
	{
		glUseProgram(0);
	}

	void Shader::setUniform1f(GLint location, float uniform) const
	{
		glUniform1f(location, uniform);
	}

	void Shader::setUniform2f(GLint location, glm::vec2 uniform) const
	{
		glUniform2f(location, uniform.x, uniform.y);
	}

	void Shader::setUniform3f(GLint location, glm::vec3 uniform) const
	{
		glUniform3f(location, uniform.x, uniform.y, uniform.z);
	}

	void Shader::setUniform4f(GLint location, glm::vec4 uniform) const
	{
		glUniform4f(location, uniform.x, uniform.y, uniform.z, uniform.w);
	}

	void Shader::setUniform1i(GLint location, int uniform) const
	{
		glUniform1i(location, uniform);
	}

	void Shader::setUniform2i(GLint location, glm::ivec2 uniform) const
	{
		glUniform2i(location, uniform.x, uniform.y);
	}

	void Shader::setUniform3i(GLint location, glm::ivec3 uniform) const
	{
		glUniform3i(location, uniform.x, uniform.y, uniform.z);
	}

	void Shader::setUniform4i(GLint location, glm::ivec4 uniform) const
	{
		glUniform4i(location, uniform.x, uniform.y, uniform.z, uniform.w);
	}

	void Shader::setUniformMat3f(GLint location, const glm::mat3& uniform) const
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, &uniform[0][0]);
	}

	void Shader::setUniformMat4f(GLint location, const glm::mat4& uniform) const
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &uniform[0][0]);
	}
}
#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace graphics {

	Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile)
	{
		std::string vertexBuffer, fragmentBuffer;
		std::stringstream streamVertex;

		std::ifstream vertexSource, fragmentSource;
		vertexSource.exceptions(std::fstream::failbit | std::fstream::badbit);
		fragmentSource.exceptions(std::fstream::failbit | std::fstream::badbit);

		vertexSource.open(vertexFile.c_str(), std::fstream::in | std::fstream::binary);
		if (!vertexSource.is_open())
		{
			std::cout << "FILE ACSESS FAIL! " << vertexFile << std::endl;
		}
		fragmentSource.open(fragmentFile.c_str(), std::fstream::in | std::fstream::binary);
		if (!fragmentSource.is_open())
		{
			std::cout << "FILE ACSESS FAIL! " << fragmentFile << std::endl;
		}

		streamVertex << vertexSource.rdbuf();
		vertexBuffer = streamVertex.str();
		streamVertex.str("");

		streamVertex << fragmentSource.rdbuf();
		fragmentBuffer = streamVertex.str();

		unsigned int vertexShader, fragmentShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertex = vertexBuffer.c_str();
		const char* fragment = fragmentBuffer.c_str();

		int success;
		char infolog[512];
			
		glShaderSource(vertexShader, 1, &vertex, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
			std::cout << "ERROR::VERTEX::SHADER::COMPILATION_FAIL!\n" << infolog << std::endl;
		}

		glShaderSource(fragmentShader, 1, &fragment, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
			std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION_FAIL!\n" << infolog << std::endl;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);

		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 512, NULL, infolog);
			std::cout << "ERROR::LINK_PROGRAMM!::FAIL!\n" << infolog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		vertexSource.close();
		fragmentSource.close();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}

	void Shader::use() const
	{
		glUseProgram(m_ID);
	}
	
	void Shader::del() const {
		glDeleteProgram(m_ID);
	}
	
	void Shader::setMat4f(const std::string &name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setVec3f(const std::string &name, glm::vec3 value) const
	{
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::setFloat(const std::string &name, GLfloat value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void Shader::setInt(const std::string &name, GLint value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
}
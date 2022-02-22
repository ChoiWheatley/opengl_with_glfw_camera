#include "Shader.h"

#include <iostream>
#include <utility>

void Shader::setUniformValue(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(getUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformValue(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(getUniformLocation(this->id, name), 1, glm::value_ptr(value));
}

void Shader::setUniformValue(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(getUniformLocation(this->id, name), 1, glm::value_ptr(value));
}

void Shader::setUniformValue(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(this->id, name), value);
}

void Shader::setUniformValue(const std::string& name, int value) const
{
	glUniform1i(getUniformLocation(this->id, name), value);
}

void Shader::setUniformValue(const std::string& name, bool value) const
{
	glUniform1i(getUniformLocation(this->id, name), value);
}

void Shader::useShaderProgram() const
{
	glUseProgram(this->id);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragPath) :
	id(glCreateProgram())
{
	try
	{
		// 1. retrieve the vertex/fragment source code from filepath
		vShaderCode = Shader::getShaderCode(vertexPath);
		fShaderCode = Shader::getShaderCode(fragPath);

		// 2. compile shader from shader code
		const unsigned int vertexShader = Shader::compileShader(this->vShaderCode.c_str(), GL_VERTEX_SHADER);
		const unsigned int fragShader = Shader::compileShader(this->fShaderCode.c_str(), GL_FRAGMENT_SHADER);

		// 3. attach to a shader program
		glAttachShader(this->id, vertexShader);
		glAttachShader(this->id, fragShader);
		linkShader(this->id);

		// 4. delete the shaders as they're linked into our program now
		glDeleteShader(vertexShader);
		glDeleteShader(fragShader);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << '\n';
		//** IMPORTANT: exception during initializing object doesn't call
		// destructor.
		glDeleteShader(this->id);
	}
}

Shader::~Shader()
{
	glDeleteShader(this->id);
}

int Shader::getUniformLocation(const unsigned id, const std::string& name)
{
	return glGetUniformLocation(id, name.c_str());
}

std::string Shader::getShaderCode(const std::string& shaderPath)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		std::stringstream shaderStream;
		shaderFile.open(shaderPath);
		shaderStream << shaderFile.rdbuf();
		std::string ret = shaderStream.str();
		shaderFile.close();

		return ret;
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << e.what() << '\n';
		throw err_read_shader_code{ "error reading shader code" };
	}
}

int Shader::compileShader(const char* shaderCode, unsigned int shaderType)
{
	int success = 0;
	char infoLog[BUFSIZ];

	const auto shader = glCreateShader(shaderType);
	if (!shader)
	{
		throw err_compile_shader{ "error create shader" };
	}
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	// throw compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, BUFSIZ, nullptr, infoLog);
		throw err_compile_shader{ infoLog };
	}
	return shader;
}

void Shader::linkShader(unsigned id)
{
	int success = 0;
	char infoLog[BUFSIZ];
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, BUFSIZ, nullptr, infoLog);
		throw err_link_shader{ infoLog };
	}
}

const char* Shader::err_log::what() const noexcept
{
	return std::runtime_error::what();
}

Shader::err_log::err_log(const char what[BUFSIZ]) : runtime_error(what)
{}

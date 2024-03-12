#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

Shader::Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	:m_VertexFilePath(vertexFilepath), m_FragmentFilePath(fragmentFilepath), m_RendererID(0)
{
	std::string vertSource = ReadShaderFile(vertexFilepath);
	std::string fragSource = ReadShaderFile(fragmentFilepath);

	m_RendererID = CreateShader(vertSource, fragSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

std::string Shader::ReadShaderFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream source;

	while (getline(stream, line))
		source << line << '\n';

	return source.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (!result)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetProgramInfoLog(program, length, &length, message);

		std::cout << "Failed to link shader!" << length << std::endl;
		std::cout << message << std::endl;

		glDeleteProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return 0;
	}

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist." << std::endl;

	m_UniformLocationCache[name] = location;

	return location;
}

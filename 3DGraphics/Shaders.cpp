#include "Shaders.h"
#include "gl_core_4_4.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	 glShaderSource(id, 1, &src, nullptr);
	 glCompileShader(id);

	/*error checking*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		 glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message =  (char*)alloca(length * sizeof(char));
		 glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		 glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vsh = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fsh = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	 glAttachShader(program, vsh);
	 glAttachShader(program, fsh);
	 glLinkProgram(program);
	 glValidateProgram(program);

	 glDeleteShader(vsh);
	 glDeleteShader(fsh);

	return program;
}

shaderProgramSource Shader::parseShaderFile(const std::string& path)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;

	std::ifstream stream(path);
	if (!stream.good())
	{
		std::cout << "Error: Could not read shader from dir: " << path << std::endl;
	}
	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	//this uses the shaderProgramSource struct in shaders.h
	return {ss[0].str(), ss[1].str()};
}

Shader::Shader(const std::string& filePath) : filePath(filePath), shaderId(0)
{

	/*parses shader and puts the id into shaderId*/
	shaderProgramSource source = parseShaderFile(filePath);
	shaderId = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(shaderId);
}

void Shader::bind() const
{
	glUseProgram(shaderId);
}

void Shader::unBind()
{
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform1i(const std::string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	//parameters: location, count, needs to be transposed, reference to float array ( since we are using a matrix we reference the column 0 row 0)
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::getUniformLocation(const std::string& name) 
{
	/*looks for current uniform in cache. If its already there, returns the uniform location int. This means we dont have to do the gl call every frame saving resources*/
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
	{
		return uniformLocationCache[name];
	}
	int location = glGetUniformLocation(shaderId, name.c_str());
	if (location == -1)
	{
		std::cout << std::endl << "SHADER PROBLEM: " << std::endl <<  "Warning: Uniform '" << name << "' doesnt exist." << std::endl;
	}
		uniformLocationCache[name] = location;
	
	return location;
}

GLuint Shader::getShaderProgramId()
{
	return shaderId;
}

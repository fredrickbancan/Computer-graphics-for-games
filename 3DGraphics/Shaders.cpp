#include "Shaders.h"
#include "gl_core_4_4.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

unsigned int Shader::compileShaderOfType(unsigned int type, const std::string& source)
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
		char* message =  (char*)_malloca(length * sizeof(char));
		 glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : type == GL_FRAGMENT_SHADER ? "fragment" : type == GL_TESS_CONTROL_SHADER ? "tess controll" : "tess eval" ) << " shader!" << std::endl;
		std::cout << message << std::endl;
		 glDeleteShader(id);
		return 0;
	}

	return id;
}
unsigned int Shader::createShader(ShaderProgramSource source)
{
	unsigned int program = glCreateProgram();
	unsigned int vertID;
	unsigned int fragID;
	unsigned int tessContID;
	unsigned int tessEvalID;

	if (source.hasFrag)
	{
		fragID = compileShaderOfType(GL_FRAGMENT_SHADER, source.fragmentSource);
		glAttachShader(program, fragID);
		glDeleteShader(fragID);
	}
	if (source.hasVert)
	{
		vertID = compileShaderOfType(GL_VERTEX_SHADER, source.vertexSource);
		glAttachShader(program, vertID);
		glDeleteShader(vertID);
	}
	if (source.hasTessCont)
	{
		tessContID = compileShaderOfType(GL_TESS_CONTROL_SHADER, source.tessContSource);
		glAttachShader(program, tessContID);
		glDeleteShader(tessContID);
	}
	if (source.hasTessEval)
	{
		tessEvalID = compileShaderOfType(GL_TESS_EVALUATION_SHADER, source.tessEvalSource);
		glAttachShader(program, tessEvalID);
		glDeleteShader(tessEvalID);
	}

	glLinkProgram(program);
	glValidateProgram(program);

	return program;
}

ShaderProgramSource Shader::parseVertFragShaderFile(const std::string& vertFragPath)
{
	enum class ShaderEnum
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderEnum type = ShaderEnum::NONE;

	std::ifstream stream(vertFragPath);
	if (!stream.good())
	{
		std::cout << "Error: Could not read fragment/vertex shader from dir: " << vertFragPath << std::endl;
	}
	std::string line;
	std::stringstream ss[2];
	ShaderProgramSource result;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderEnum::VERTEX;
				result.hasVert = true;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderEnum::FRAGMENT;
				result.hasFrag = true;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	stream.close();

	result.fragmentSource = ss[1].str();
	result.vertexSource = ss[0].str();

	//this uses the shaderProgramSource struct in shaders.h
	return result;
}

ShaderProgramSource Shader::parseTessShaderFile(const std::string& tessPath, ShaderProgramSource fragVertSource)
{
	enum class ShaderEnum
	{
		NONE = -1, CONT = 0, EVAL = 1
	};

	ShaderEnum type = ShaderEnum::NONE;

	std::ifstream stream(tessPath);
	if (!stream.good())
	{
		std::cout << "Error: Could not read tess shader from dir: " << tessPath << std::endl;
	}
	std::string line;
	std::stringstream ss[2];
	ShaderProgramSource result = fragVertSource;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("tesscont") != std::string::npos)
			{
				type = ShaderEnum::CONT;
				result.hasTessCont = true;
			}
			else if (line.find("tesseval") != std::string::npos)
			{
				type = ShaderEnum::EVAL;
				result.hasTessEval = true;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	stream.close();

	result.tessContSource = ss[0].str();
	result.tessEvalSource = ss[1].str();

	//this uses the shaderProgramSource struct in shaders.h
	return result;
}

Shader::Shader(const std::string& fragVertPath) : vertFragFilePath(fragVertPath), shaderId(0)
{
	ShaderProgramSource source = parseVertFragShaderFile(fragVertPath);
	shaderId = createShader(source);
}

Shader::Shader(const std::string& vertFragFilePath, const std::string& tessFilePath) : vertFragFilePath(vertFragFilePath), tessFilePath(tessFilePath), shaderId(0)
{
	ShaderProgramSource source = parseVertFragShaderFile(vertFragFilePath);
	source = parseTessShaderFile(tessFilePath, source);
	shaderId = createShader(source);
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

void Shader::setUniform4f(const std::string& name, glm::vec4 vec)
{
	glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform3f(const std::string& name, glm::vec3 vec)
{
	glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::setUniform1i(const std::string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string& name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	//parameters: location, count, needs to be transposed, reference to float array ( since we are using a matrix we reference the column 0 row 0)
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformMat4fArray(const std::string& name, int count, const float* begin)
{
	//parameters: location, count, needs to be transposed, reference to float array ( since we are using a matrix we reference the column 0 row 0)
	glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, begin);
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

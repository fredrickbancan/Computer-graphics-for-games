#pragma once
//#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "glm/mat4x4.hpp"

struct shaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
    unsigned int shaderId;
    std::string filePath;
    std::unordered_map<std::string, int> uniformLocationCache;
private:
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int compileShader(unsigned int type, const std::string& source);
    shaderProgramSource parseShaderFile(const std::string& path);
public:
    Shader(const std::string& filePath);
    ~Shader();
    void bind() const;
    void unBind();
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string& name, int value);
    void setUniformMat4f(const std::string& name, const glm::mat4 matrix );
    int getUniformLocation(const std::string& name);
    unsigned int getShaderProgramId();
};
#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "glm/mat4x4.hpp"

/*Simple struct for containing multiple different types of shader code.*/
struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
    std::string tessContSource;
    std::string tessEvalSource;

    bool hasVert = false;
    bool hasFrag = false;
    bool hasTessCont = false;
    bool hasTessEval = false;
};

class Shader
{
private:
    unsigned int shaderId;
    std::string vertFragFilePath;
    std::string tessFilePath;
    /*unordered map for uniforms. Previously called uniforms can be found in this map, which is faster than repeatidly searching large shader source code for a single uniform.*/
    std::unordered_map<std::string, int> uniformLocationCache;
private:

    /*Calls all opengl compilation functions for each part of legitimate shader source code in provided source struct*/
    unsigned int createShader(ShaderProgramSource source);

    /*calls compilation functions for provided shader type num and source*/
    unsigned int compileShaderOfType(unsigned int type, const std::string& source);

    /*Parses vertex/fragment shader file and returns shaderprogramsource struct with found source code*/
    ShaderProgramSource parseVertFragShaderFile(const std::string& path);

    /*Parses tesselation shader file and adds its code to the provided shaderprogramsource struct, then returns it.*/
    ShaderProgramSource parseTessShaderFile(const std::string& tessPath, ShaderProgramSource fragVertSource);
public:
    /*Constructor for shader without tesselation*/
    Shader(const std::string& vertFragFilePath);

    /*Constructor for shader with tesselation*/
    Shader(const std::string& vertFragFilePath, const std::string& tessFilePath);
    
    ~Shader();
    
    void bind() const;
    void unBind();
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform4f(const std::string& name, glm::vec4 vec);
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
    void setUniform3f(const std::string& name, glm::vec3 vec);
    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniformMat4f(const std::string& name, const glm::mat4 matrix );
    void setUniformMat4fArray(const std::string& name, int count, const float* begin);
    int getUniformLocation(const std::string& name);
    unsigned int getShaderProgramId();
};
#ifndef SHADER_H
#define SHADER_H
// Shader.h

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    std::string m_Filepath; // Path to the shader file
    unsigned int m_RendererId; // OpenGL shader program ID
    std::unordered_map<std::string, int> m_UniformLocationCache; // caching for uniforms
public:
    Shader(const std::string& filename);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private: 
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int GetUniformLocation(const std::string& name);
};

#endif // SHADER_H
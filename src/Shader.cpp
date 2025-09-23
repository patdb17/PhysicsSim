#include "Shader.h"
#include "Renderer.h"

#include <fstream>

Shader::Shader(const std::string &filename)
    : m_Filepath(filename), m_RendererId(0)
{
    ShaderProgramSource shaderSource = ParseShader(m_Filepath);
    m_RendererId = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    
}

Shader::~Shader()
{
    GL_CALL(glDeleteProgram(m_RendererId));
    LOG(LogLevel::INFO, "Shader {} destroyed", m_Filepath);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GL_CALL(glShaderSource(id, 1, &src, nullptr));
    GL_CALL(glCompileShader(id));

    // TODO: Error handling 
    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(id, length, &length, message));
        LOG(LogLevel::ERROR, "Failed to compile shader of type {}: {}", 
            (type == GL_VERTEX_SHADER) ? "vertex" : "fragment", message);

        GL_CALL(glDeleteShader(id));
        return 0; // Return 0 to indicate failure
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if (!stream.is_open())
    {
        LOG(LogLevel::ERROR, "Could not open shader file: {}", filepath);
        return { "", ""};
    }

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else if (type != ShaderType::NONE)
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[static_cast<int>(ShaderType::VERTEX)].str(), 
             ss[static_cast<int>(ShaderType::FRAGMENT)].str()};
}

void Shader::Bind() const
{
    GL_CALL(glUseProgram(m_RendererId));
    //LOG(LogLevel::DEBUG, "Shader {} bound", m_Filepath);
}

void Shader::Unbind() const
{
    GL_CALL(glUseProgram(0));
    LOG(LogLevel::DEBUG, "Shader {} unbound", m_Filepath);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.contains(name))
    {
        return m_UniformLocationCache[name];
    }
    // Set up a uniform variable in the shader
    // Assuming the shader has a uniform variable named "u_Color" of type vec4
    GL_CALL(int location = glGetUniformLocation(m_RendererId, name.c_str()));
    if (location == -1) // Ensure the uniform location is valid
    {
        LOG(LogLevel::WARNING, "Uniform {} doesn't exist!", name);
    }
    
    m_UniformLocationCache[name] = location;

    return location;
    //GL_CALL(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f)); // Set the color uniform
}
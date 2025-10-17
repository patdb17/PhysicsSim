#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H
// VertexBufferLayout.h

#include <vector>

#include <GL/glew.h>

#include "Logger.h"
#include "OpenGlUtils.h"

struct VertexBufferElement
{
    unsigned int  type;
    unsigned int  count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:         return sizeof(GLfloat);
            case GL_UNSIGNED_INT:  return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
            default:
                // Log an error message if an unsupported type is encountered
                PrintMessageNow(LogLevel::ERROR, __LINE__, __FILE__, 
                    std::format("Unsupported type in VertexBufferElement: {}", type));              
                ASSERT(false); // Unsupported type
                return 0; // Unsupported type
        }
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int                     m_Stride = 0;

public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    template<typename T>
    void Push(unsigned int count)
    {
        // static_assert(false);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElements() const
    {
        return m_Elements;
    }

    inline unsigned int GetStride() const
    {
        return m_Stride;
    }
};

#endif

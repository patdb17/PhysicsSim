#include "VertexArray.h"

#include "OpenGlUtils.h"

VertexArray::VertexArray()
{
    // Modern OpenGL requires a VAO be defined and bound if you are using the core profile
    GL_CALL(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind(); // Bind the Vertex Array Object
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i)); // Enable the vertex attribute at index 0
        GL_CALL(glVertexAttribPointer(i, element.count, element.type, 
            element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GL_CALL(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
    GL_CALL(glBindVertexArray(0));
}

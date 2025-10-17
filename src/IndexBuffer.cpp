#include "IndexBuffer.h"

#include "OpenGlUtils.h"

IndexBuffer::IndexBuffer(const unsigned int *data, const unsigned int count)
    : m_Count(count)
{
    // Create a index array buffer to put data into
    GL_CALL(glGenBuffers(1, &m_RendererId));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

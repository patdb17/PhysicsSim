#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    // Create a vertex array buffer to put data into
    GL_CALL(glGenBuffers(1, &m_RendererId));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
// VertexArray.h

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererId; // OpenGL buffer ID

    // Store the layout of the vertex attributes
    std::vector<VertexBufferLayout> m_Layouts;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};

#endif // !VERTEX_ARRAY_H
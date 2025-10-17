#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H
// VertexBuffer.h

class VertexBuffer
{
private:
    unsigned int m_RendererId; // OpenGL buffer ID

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void UpdateData(const void* data, unsigned int size);
};

#endif // !VERTEX_BUFFER_H

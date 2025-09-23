#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H
// IndexBuffer.h

class IndexBuffer
{
private:
    unsigned int m_RendererId; // OpenGL buffer ID
    unsigned int m_Count;      // Number of indices in the buffer

public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; } // Get the number of indices in the buffer
};

#endif // !INDEX_BUFFER_H

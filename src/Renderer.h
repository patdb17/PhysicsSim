#ifndef RENDERER_H
#define RENDERER_H
// Renderer.h

// C++ Standard Library includes
#include <string>

// External Libraries includes
#include <GL/glew.h>

// Project includes
#include "Logger.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
    public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};


#endif // !RENDERER_H

#include "VertexBuffer.h"
#include "../Debugging.h"

namespace Graphics
{
    VertexBuffer::VertexBuffer(const void* data, uint size) {
        GLCALL(glGenBuffers(1, &rendererID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer() {
        GLCALL(glDeleteBuffers(1, &rendererID));
    }

    void VertexBuffer::Bind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    }

    void VertexBuffer::Unbind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}
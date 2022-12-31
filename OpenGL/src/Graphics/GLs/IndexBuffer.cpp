#include "IndexBuffer.h"
#include "Debugging.h"

namespace Graphics
{
    IndexBuffer::IndexBuffer(const void* data, unsigned int length) : length(length) {
        GLCALL(glGenBuffers(1, &rendererID));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
        GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(unsigned int), data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer() {
        GLCALL(glDeleteBuffers(1, &rendererID));
    }

    void IndexBuffer::Bind() const {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    }

    void IndexBuffer::Unbind() const {
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}
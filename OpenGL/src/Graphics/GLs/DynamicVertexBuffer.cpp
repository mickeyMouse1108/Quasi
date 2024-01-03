#include "DynamicVertexBuffer.h"

namespace Graphics {
    DynamicVertexBuffer::DynamicVertexBuffer(uint size, uint typeSize, std::type_index type)
        : bufferSize(size), vertSize(typeSize), vertType(type) {
        GLCALL(glGenBuffers(1, &rendererID));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, typeSize * size, nullptr, GL_DYNAMIC_DRAW));
    }

    DynamicVertexBuffer::~DynamicVertexBuffer() {
        GLCALL(glDeleteBuffers(1, &rendererID));
    }

    DynamicVertexBuffer& DynamicVertexBuffer::Transfer(DynamicVertexBuffer& dest, DynamicVertexBuffer&& from) {
        dest.rendererID = from.rendererID;
        from.rendererID = 0;
        dest.dataOffset = from.dataOffset;
        dest.bufferSize = from.bufferSize;

        dest.vertSize = from.vertSize;
        dest.vertType = from.vertType;

        return dest;
    }

    void DynamicVertexBuffer::Bind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    }

    void DynamicVertexBuffer::Unbind() const {
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void DynamicVertexBuffer::SetDataUnchecked(const void* data, uint vertSize, uint count) {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * vertSize, data);
    }

    void DynamicVertexBuffer::ClearData(bool shallowClear) {
        Bind();
        dataOffset = 0;
        if (shallowClear) return;
        const std::vector<uchar> clear(bufferSize * vertSize, 0);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * vertSize, clear.data());
    }

    void DynamicVertexBuffer::AddDataUnchecked(const void* data, uint vertSize, uint count) {
        glBufferSubData(GL_ARRAY_BUFFER, dataOffset * vertSize, count * vertSize, data);
        dataOffset += count;
    }
}
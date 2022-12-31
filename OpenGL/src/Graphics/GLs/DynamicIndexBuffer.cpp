#include "DynamicIndexBuffer.h"

#include <algorithm>
#include <vector>
#include <GL/glew.h>

namespace Graphics
{
    DynamicIndexBuffer::DynamicIndexBuffer(unsigned int size) : bufferSize(size)
    {
        glGenBuffers(1, &rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size, nullptr, GL_DYNAMIC_DRAW);
    }

    DynamicIndexBuffer::~DynamicIndexBuffer()
    {
        glDeleteBuffers(1, &rendererID);
    }

    void DynamicIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    }

    void DynamicIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void DynamicIndexBuffer::SetData(const unsigned* data, unsigned size)
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(unsigned int), data);
    }

    void DynamicIndexBuffer::SetDataWhole(const unsigned* data)
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferSize * sizeof(unsigned), data);
    }

    void DynamicIndexBuffer::ClearData(bool shallowClear)
    {
        Bind();
        dataOffset = 0;
        indexOffset = 0;
        if (shallowClear) return;
        unsigned* clear = new unsigned[bufferSize];
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferSize * sizeof(unsigned), clear);
        delete[] clear;
    }

    void DynamicIndexBuffer::AddData(const unsigned* data, unsigned size, unsigned int indexMax)
    {
        std::vector<unsigned> dataOff(size);
        for (unsigned i = 0; i < size; ++i) dataOff[i] = data[i] + indexOffset;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, dataOffset * sizeof(unsigned), size * sizeof(unsigned), dataOff.data());
        dataOffset += size;
        indexOffset += indexMax;
    }

    void DynamicIndexBuffer::AddData(const unsigned& data)
    {
        unsigned int dataOff = data + indexOffset;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, dataOffset * sizeof(unsigned), sizeof(unsigned), (const void*)dataOff);
        ++dataOffset;
        indexOffset = std::max(dataOff, indexOffset) + 1;
    }
}

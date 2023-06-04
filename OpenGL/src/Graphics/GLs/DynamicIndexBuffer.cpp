#include "DynamicIndexBuffer.h"

#include <algorithm>
#include <vector>
#include <GL/glew.h>

namespace Graphics {
    DynamicIndexBuffer::DynamicIndexBuffer(unsigned int size) : bufferSize(size) {
        glGenBuffers(1, &rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size, nullptr, GL_DYNAMIC_DRAW);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    }

    DynamicIndexBuffer::~DynamicIndexBuffer() {
        glDeleteBuffers(1, &rendererID);
    }

    void DynamicIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    }

    void DynamicIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void DynamicIndexBuffer::SetData(const uint* data, uint size) {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(unsigned int), data);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    }

    void DynamicIndexBuffer::SetDataWhole(const unsigned* data) {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferSize * sizeof(unsigned), data);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    }

    void DynamicIndexBuffer::ClearData(bool shallowClear) {
        Bind();
        dataOffset = 0;
        indexOffset = 0;
        if (shallowClear) return;
        const std::vector<uint> clear { bufferSize, 0 };
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferSize * sizeof(unsigned), clear.data());  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    }

    void DynamicIndexBuffer::AddData(const uint* data, uint size, uint maxIndex) {
        std::vector<uint> dataOff(size);
        const uint indexOff = indexOffset;
        std::transform(data, data + size, dataOff.begin(), [=](uint i){ return i + indexOff; } );
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, dataOffset * sizeof(unsigned), size * sizeof(unsigned), dataOff.data());  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
        dataOffset += size;
        indexOffset += maxIndex;
    }

    void DynamicIndexBuffer::AddData(uint data) {
        const uint dataOff = data + indexOffset;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, dataOffset * sizeof(unsigned), sizeof(unsigned), (const void*)dataOff);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions, performance-no-int-to-ptr)
        ++dataOffset;
        indexOffset = std::max(dataOff, indexOffset) + 1;
    }
}

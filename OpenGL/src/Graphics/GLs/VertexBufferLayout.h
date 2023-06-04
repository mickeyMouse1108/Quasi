#pragma once
#include <vector>
#include <GL/glew.h>

#include "Debugging.h"
#include "NumTypes.h"

namespace Graphics {
    struct VertexBufferElement {
        uint type;
        uint count;
        uchar normalized;

        static uint sizeofType(unsigned int type) {
            switch (type) {
                case GL_FLOAT:
                case GL_UNSIGNED_INT:  return 4;
                case GL_UNSIGNED_BYTE: return 1;
                default:               { ASSERT(true); return 0;}
            }
        }
    };

    class VertexBufferLayout {
        private:
            std::vector<VertexBufferElement> _elements;
            uint stride;
        public:
            VertexBufferLayout();

            template <typename T> void Push(uint count) = delete;

            [[nodiscard]] const std::vector<VertexBufferElement>& GetElements() const { return _elements; }
            [[nodiscard]] uint GetStride() const { return stride; }
    };

    template <> void VertexBufferLayout::Push<float>(uint count);
    template <> void VertexBufferLayout::Push<uint>(uint count);
    template <> void VertexBufferLayout::Push<uchar>(uint count);
}

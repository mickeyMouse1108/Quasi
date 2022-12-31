#pragma once
#include <vector>
#include <GL/glew.h>

#include "Debugging.h"

namespace Graphics
{
    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        unsigned char normalized;

        static unsigned int sizeofType(unsigned int type)
        {
            switch (type)
            {
            case GL_FLOAT:
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;
            default:               { ASSERT(true); return 0;}
            }
        }
    };

    class VertexBufferLayout
    {
    private:
        std::vector<VertexBufferElement> _elements;
        unsigned int stride;
    public:
        VertexBufferLayout();

        template <typename T> void Push(unsigned int count) = delete;
        template <> void Push<float>(unsigned int count);
        template <> void Push<unsigned int>(unsigned int count);
        template <> void Push<unsigned char>(unsigned int count);

        inline const std::vector<VertexBufferElement>& GetElements() const { return _elements; }
        inline unsigned int GetStride() const { return stride; }
    };
}
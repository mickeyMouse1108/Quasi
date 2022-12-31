#include "VertexBufferLayout.h"

namespace Graphics
{
    VertexBufferLayout::VertexBufferLayout() : stride(0)
    {
    }


    template <>
    void VertexBufferLayout::Push<float>(unsigned int const count)
    {
        _elements.push_back({GL_FLOAT, count, GL_FALSE});
        stride += count * VertexBufferElement::sizeofType(GL_FLOAT);
    }

    template <>
    void VertexBufferLayout::Push<unsigned int>(unsigned int const count)
    {
        _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        stride += count * VertexBufferElement::sizeofType(GL_UNSIGNED_INT);
    }

    template <>
    void VertexBufferLayout::Push<unsigned char>(unsigned int const count)
    {
        _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        stride += count * VertexBufferElement::sizeofType(GL_UNSIGNED_BYTE);
    }
}
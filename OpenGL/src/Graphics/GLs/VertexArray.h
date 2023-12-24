#pragma once
#include "DynamicVertexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexElement.h"

namespace Graphics {
    class VertexArray {
        private:
            uint rendererID = 0;
        public:
            OPENGL_API VertexArray();
            OPENGL_API ~VertexArray();

            OPENGL_API void AddBuffer(const VertexBufferLayout& layout);
            OPENGL_API void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
            template <class T>
            OPENGL_API void AddBuffer(const DynamicVertexBuffer<T>& vb);

            OPENGL_API void Bind() const;
            OPENGL_API void Unbind() const;

        template <class T>
        using VBO = const DynamicVertexBuffer<T>&;
    };

    template <class T>
    void VertexArray::AddBuffer(const DynamicVertexBuffer<T>& vb) {
        Bind();
        vb.Bind();
        this->AddBuffer(VERTEX_LAYOUT_OF(T));
    }
}

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
            VertexArray();
            ~VertexArray();
        
            void AddBuffer(const VertexBufferLayout& layout);
            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
            template <class T>
            void AddBuffer(const DynamicVertexBuffer<T>& vb);
            
            void Bind() const;
            void Unbind() const;

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

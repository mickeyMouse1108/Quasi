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

            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
            
            template <class T>
            void AddBuffer(const DynamicVertexBuffer<T>& vb) = delete;
            
            void Bind() const;
            void Unbind() const;

        template <class T>
        using VBO = const DynamicVertexBuffer<T>&;
    };

    template <>
    void VertexArray::AddBuffer(VBO<float> vb);
    template <>
    void VertexArray::AddBuffer(VBO<VertexColorTexture3D> vb);
    template <>
    void VertexArray::AddBuffer(VBO<VertexColorTextureAtlas3D> vb);
    template <>
    void VertexArray::AddBuffer(VBO<VertexColor3D> vb);

    
}
#pragma once
#include "DynamicVertexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexElement.h"

namespace Graphics
{
    class VertexArray
    {
    private:
        unsigned int rendererID;
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        
        template <typename T>
        void AddBuffer(const DynamicVertexBuffer<T>& vb) = delete;
        template <>
        void AddBuffer<float>(const DynamicVertexBuffer<float>& vb);
        template <>
        void AddBuffer<VertexColorTexture3D>(const DynamicVertexBuffer<VertexColorTexture3D>& vb);
        template <>
        void AddBuffer<VertexColor3D>(const DynamicVertexBuffer<VertexColor3D>& vb);
        
        void Bind() const;
        void Unbind() const;
    };
}
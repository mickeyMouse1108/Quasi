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

            VertexArray(const VertexArray&) = delete;
            VertexArray& operator=(const VertexArray&) = delete;
            OPENGL_API static VertexArray& Transfer(VertexArray& dest, VertexArray&& from);
            VertexArray(VertexArray&& va) noexcept { Transfer(*this, std::move(va)); }
            VertexArray& operator=(VertexArray&& va) noexcept { return Transfer(*this, std::move(va)); }

            OPENGL_API void AddBuffer(const VertexBufferLayout& layout);
            OPENGL_API void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
            OPENGL_API void AddBuffer(const DynamicVertexBuffer& vb, const VertexBufferLayout& layout);

            OPENGL_API void Bind() const;
            OPENGL_API void Unbind() const;

    };
}

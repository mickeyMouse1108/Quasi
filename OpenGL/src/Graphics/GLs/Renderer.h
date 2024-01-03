#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderData.h"

namespace Graphics {
    class Renderer {
        public:
            OPENGL_API void Clear() const;
            OPENGL_API void Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader) const;
            OPENGL_API void Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader) const;
            void Draw(const RenderData& dat, const Shader& s) const { Draw(dat.GetVertArr(), dat.GetIndObj(), s); }

            OPENGL_API void SetRenderWireframe(bool isWireframe);
    };
}

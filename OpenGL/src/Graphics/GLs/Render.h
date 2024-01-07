#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderData.h"

namespace Graphics::Render {
    OPENGL_API void Clear();
    OPENGL_API void Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader);
    OPENGL_API void Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader);
    inline void Draw(const RenderData& dat, const Shader& s) { Draw(dat.GetVertArr(), dat.GetIndObj(), s); }

    OPENGL_API void SetRenderWireframe(bool isWireframe);
    OPENGL_API void SetClearColor(const Maths::fvec4& color);
}

#pragma once
#include "DynamicIndexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Graphics {
    class Renderer {
        private:
            uint renderType = GL_TRIANGLES;
        public:
            OPENGL_API void Clear() const;
            OPENGL_API void Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader) const;
            OPENGL_API void Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader) const;

            OPENGL_API void SetRenderMode(uint drawType);
            OPENGL_API void SetRenderMode(char drawType); // t: tri, w: wireframe, 
    };
}

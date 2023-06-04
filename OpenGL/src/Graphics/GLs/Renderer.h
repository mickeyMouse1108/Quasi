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
            void Clear() const;
            void Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader) const;
            void Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader) const;

            void SetRenderMode(uint drawType);
            void SetRenderMode(char drawType); // t: tri, w: wireframe, 
    };
}

#include "Renderer.h"

namespace Graphics {
    void Renderer::Clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Renderer::Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader) const {
        vertexArr.Bind();
        indexBuff.Bind();
        shader.Bind();
        GLCALL(glDrawElements(GL_TRIANGLES, indexBuff.GetLength(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader) const {
        vertexArr.Bind();
        indexBuff.Bind();
        shader.Bind();
        GLCALL(glDrawElements(GL_TRIANGLES, indexBuff.GetUsedLength(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::SetRenderWireframe(bool isWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);
    }
}

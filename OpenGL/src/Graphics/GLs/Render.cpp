#include "Render.h"

namespace Graphics::Render {
    void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader) {
        vertexArr.Bind();
        indexBuff.Bind();
        shader.Bind();
        GLCALL(glDrawElements(GL_TRIANGLES, indexBuff.GetLength(), GL_UNSIGNED_INT, nullptr));
    }

    void Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader) {
        vertexArr.Bind();
        indexBuff.Bind();
        shader.Bind();
        GLCALL(glDrawElements(GL_TRIANGLES, indexBuff.GetUsedLength(), GL_UNSIGNED_INT, nullptr));
    }

    void SetRenderWireframe(bool isWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);
    }

    void SetClearColor(const Maths::colorf& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }
}

#include "Renderer.h"

namespace Graphics
{
    void Renderer::Clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Renderer::Draw(const VertexArray& vertexArr, const IndexBuffer& indexBuff, const Shader& shader) const
    {
        vertexArr.Bind();
        indexBuff.Bind();
        shader.Bind();
        GLCALL(glDrawElements(GL_TRIANGLES, indexBuff.GetLength(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::Draw(const VertexArray& vertexArr, const DynamicIndexBuffer& indexBuff, const Shader& shader) const
    {
        vertexArr.Bind();
        indexBuff.Bind();
        shader.Bind();
        GLCALL(glDrawElements(GL_TRIANGLES, indexBuff.GetLength(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::SetRenderMode(unsigned drawType)
    {
        renderType = drawType;
    }

    void Renderer::SetRenderMode(char drawType)
    {
        switch(drawType)
        {
        case 't': renderType = GL_TRIANGLES; return;
        case 'w': renderType = GL_LINES; return;
        default:;
        }
    }
}

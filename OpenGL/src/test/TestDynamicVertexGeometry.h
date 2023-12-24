#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "Test.h"
#include "VertexArray.h"

namespace Test
{
    class TestDynamicVertexGeometry : public Test
    {
    private:
        VertexColor3D* vertexes;
        
        Graphics::DynamicVertexBuffer<VertexColor3D>* vb;
        Graphics::VertexArray* va;
        Graphics::IndexBuffer* ib;
        Graphics::Shader* shader;

        Maths::Matrix3D projection;
    public:
        OPENGL_API TestDynamicVertexGeometry();
        OPENGL_API ~TestDynamicVertexGeometry() override;

        OPENGL_API void OnUpdate(float deltaTime) override;
        OPENGL_API void OnRender(Graphics::Renderer& renderer) override;
        OPENGL_API void OnImGuiRender() override;
    };
}

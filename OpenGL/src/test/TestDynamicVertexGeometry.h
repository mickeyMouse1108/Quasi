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
        TestDynamicVertexGeometry();
        ~TestDynamicVertexGeometry() override;

        void OnUpdate(float deltaTime) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
    };
}

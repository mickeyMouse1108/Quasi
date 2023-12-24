#pragma once
#include "Test.h"

namespace Test
{
    class TestBatchedColored : public Test
    {
    private:
        Graphics::VertexBuffer* vb;
        Graphics::VertexArray* va;
        Graphics::IndexBuffer* ib;
        Graphics::Shader* shader;
        
        Maths::Matrix3D projection;
        Maths::Vector3  modelTranslation;
        Maths::Vector3  modelScale;
        Maths::Vector3  modelRotation;
    public:
        OPENGL_API TestBatchedColored();
        OPENGL_API ~TestBatchedColored() override;
        OPENGL_API void OnUpdate(float deltaTime) override;
        OPENGL_API void OnRender(Graphics::Renderer& renderer) override;
        OPENGL_API void OnImGuiRender() override;
    };
}

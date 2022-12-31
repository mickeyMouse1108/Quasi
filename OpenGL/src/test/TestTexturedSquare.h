#pragma once
#include "Test.h"
#include "Texture.h"

namespace Test
{
    class TestTexturedSquare : public Test
    {
    private:
        Graphics::Texture* texture;
        
        Graphics::VertexBuffer* vb;
        Graphics::VertexArray* va;
        Graphics::IndexBuffer* ib;
        Graphics::Shader* shader;
        
        float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        Maths::Matrix3D projection;
        Maths::Vector3  modelTranslation;
        Maths::Vector3  modelScale;
        Maths::Vector3  modelRotation;
    public:
        TestTexturedSquare();
        ~TestTexturedSquare() override;
        void OnUpdate(float deltaTime) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
    };
}

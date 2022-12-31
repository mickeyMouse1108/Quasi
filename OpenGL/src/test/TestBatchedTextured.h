#pragma once
#include "Test.h"
#include "Texture.h"

namespace Test
{
    class TestBatchedTextured : public Test
    {
    private:
        Graphics::Texture* textures[2];
        
        Graphics::VertexBuffer* vb;
        Graphics::VertexArray* va;
        Graphics::IndexBuffer* ib;
        Graphics::Shader* shader;
        
        Maths::Matrix3D projection;
        Maths::Vector3  modelTranslation;
        Maths::Vector3  modelScale;
        Maths::Vector3  modelRotation;
    public:
        TestBatchedTextured();
        ~TestBatchedTextured() override;
        
        void OnUpdate(float deltaTime) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
    };
}

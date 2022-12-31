#pragma once
#include "QuadMesh.h"
#include "Test.h"

namespace Test
{
    class TestCubeRender : public Test
    {
    private:
        // unsigned int faceOrder[6] = { 0, 1, 2, 3, 4, 5 };
        Graphics::QuadMesh<VertexColor3D> cubeFaces[6];
        
        Graphics::DynamicVertexBuffer<VertexColor3D>* vb;
        Graphics::DynamicIndexBuffer* ib;
        Graphics::VertexArray* va;
        Graphics::Shader* shader;

        Maths::Matrix3D projection;
        Maths::Vector3 modelTranslation = {0.0f, 0.0f, 0.0f};
        Maths::Vector3 modelScale       = {1.0f, 1.0f, 1.0f};
        Maths::Vector3 modelRotation    = {0.0f, 0.0f, 0.0f};
        float alpha = 1.0f;
    public:
        TestCubeRender();
        ~TestCubeRender() override;
        
        void OnUpdate(float deltaTime) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
        // static Maths::Vector3 faceAxis[6];
    };
}

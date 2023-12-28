#pragma once
#include "DynamicIndexBuffer.h"
#include "Test.h"
#include "Mesh.h"

namespace Test
{
    class TestDynamicTris : public Test
    {
    private:
        static constexpr int MAX_TRIS = 8;

        bool isMax = false;
        bool isMin = false;

        uint triCount = 1;
        
        Graphics::Mesh<VertexColor3D> currentTri;
        Graphics::Mesh<VertexColor3D> tris[MAX_TRIS];
        
        Graphics::DynamicVertexBuffer<VertexColor3D>* vb;
        Graphics::DynamicIndexBuffer* ib;
        Graphics::VertexArray* va;
        Graphics::Shader* shader;

        Maths::Matrix3D projection;
        Maths::Vector2 modelTranslation = {0.0f, 0.0f};
        Maths::Vector2 modelScale = {1.0f, 1.0f};
        float modelRotation = 0.0f;

        Maths::Matrix3D ModelMatrix();
    public:
        OPENGL_API TestDynamicTris();
        OPENGL_API ~TestDynamicTris() override;

        OPENGL_API void OnUpdate(float deltaTime) override;
        OPENGL_API void OnRender(Graphics::Renderer& renderer) override;
        OPENGL_API void OnImGuiRender() override;

        static Maths::Vector4 COLORS[8];
    };
}

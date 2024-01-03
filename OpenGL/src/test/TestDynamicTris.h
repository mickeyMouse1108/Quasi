#pragma once
#include "Test.h"
#include "Mesh.h"

namespace Test {
    class TestDynamicTris : public Test {
    private:
        static constexpr int MAX_TRIS = 8;

        bool isMax = false;
        bool isMin = false;
        
        stdu::ref<Graphics::RenderData> render;
        std::vector<Graphics::Mesh<VertexColor3D>> tris;
        
        Maths::Matrix3D projection = Maths::Matrix3D::OrthoProjection(-320.f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        Maths::Vector2 modelTranslation = {0.0f, 0.0f};
        Maths::Vector2 modelScale = {1.0f, 1.0f};
        float modelRotation = 0.0f;

        Maths::Matrix3D ModelMatrix();
    public:
        TestDynamicTris() {}
        ~TestDynamicTris() override {}

        OPENGL_API void OnInit(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnDestroy(Graphics::GraphicsDevice& gdevice) override;

        OPENGL_API Graphics::Mesh<VertexColor3D> NewTri();

        static Maths::Vector4 COLORS[8];
    };
}

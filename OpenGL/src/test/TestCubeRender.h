#pragma once
#include "Mesh.h"
#include "Test.h"

namespace Test {
    class TestCubeRender : public Test {
    private:
        // unsigned int faceOrder[6] = { 0, 1, 2, 3, 4, 5 };
        stdu::ref<Graphics::RenderData> render;
        Graphics::Mesh<VertexColor3D> cube;

        Maths::Matrix3D projection = Maths::Matrix3D::OrthoProjection(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
        Maths::Vector3 modelTranslation = 0;
        Maths::Vector3 modelScale       = Maths::Vector3::ONE;
        Maths::Vector3 modelRotation    = 0;
        float alpha = 1.0f;
    public:
        TestCubeRender() {}
        ~TestCubeRender() override {}

        OPENGL_API void OnInit(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

#pragma once
#include "Test.h"
#include "Texture.h"
#include "Mesh.h"

namespace Test {
    class TestTexturedSquare : public Test {
    private:
        stdu::ref<Graphics::RenderData> render;
        Graphics::Mesh<VertexColorTexture3D> mesh;
        Graphics::Texture texture;
        
        Maths::Vector4 color = Maths::Vector4::ONE;

        Maths::Matrix3D projection = Maths::Matrix3D::OrthoProjection(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        Maths::Vector3  modelTranslation = 0;
        Maths::Vector3  modelScale = Maths::Vector3::ONE;
        Maths::Vector3  modelRotation = 0;
    public:
        TestTexturedSquare() {}
        ~TestTexturedSquare() override {}

        OPENGL_API void OnInit(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

﻿#pragma once
#include "Test.h"
#include "Mesh.h"

namespace Test {
    class TestDynamicVertexGeometry : public Test {
    private:
        Graphics::RenderObject<Graphics::VertexColor2D> render;
        Graphics::Mesh<Graphics::VertexColor2D> mesh;

        Math::Matrix3D projection = Math::Matrix3D::ortho_projection({ -320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f });

        DEFINE_TEST_T(TestDynamicVertexGeometry, BASIC)
    public:
        TestDynamicVertexGeometry() = default;

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override {}
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

﻿#pragma once

#include "Test.h"
#include "Mesh.h"

namespace Test {
    class TestDynamicQuadGeometry : public Test {
    private:
        static constexpr u32 MAX_QUAD = 12;
        static constexpr u32 MAX_VERTEX = MAX_QUAD * 4;
        static constexpr u32 MAX_INDEX = MAX_QUAD * 6;
        
        bool isMin = false;
        bool isMax = false;

        Graphics::RenderObject<Graphics::VertexColor2D> render;
        Vec<Graphics::Mesh<Graphics::VertexColor2D>> quads;

        Math::Matrix3D projection = Math::Matrix3D::ortho_projection({ -320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f });

        DEFINE_TEST_T(TestDynamicQuadGeometry, BASIC)
    public:
        TestDynamicQuadGeometry() = default;

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override {}
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;

        Graphics::Mesh<Graphics::VertexColor2D> NewQuad();
    };
}

﻿#pragma once
#include "Test.h"
#include "GLs/Texture.h"
#include "../../../Quasi/src/Graphics/Mesh.h"

namespace Test {
    class TestBatchedTextured : public Test {
    public:
        struct Vertex {
            Math::fv2 Position;
            Math::fColor   Color;
            Math::fv2 TextureCoordinate;
            int TextureID;

            QuasiDefineVertex$(Vertex, 2D, (Position, Position)(Color)(TextureCoordinate)(TextureID));
        };
    private:
        Graphics::RenderObject<Vertex> render;
        Graphics::Mesh<Vertex> mesh;
        Graphics::Texture textures[2];

        Math::fColor color = 1;

        Math::Matrix3D projection = Math::Matrix3D::OrthoProjection({ { -320, -240, -1 }, { 320, 240, 1 } });

        DEFINE_TEST_T(TestBatchedTextured, BASIC)
    public:
        TestBatchedTextured() = default;

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override {}
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

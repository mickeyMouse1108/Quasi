﻿#pragma once
#include "Test.h"
#include "Textures/Texture.h"
#include "Mesh.h"

namespace Test {
    class TestTexturedSquare : public Test {
    private:
        Graphics::RenderObject<VertexColorTexture3D> render;
        Graphics::Mesh<VertexColorTexture3D> mesh;
        Graphics::Texture texture;
        
        Maths::colorf color = Maths::colorf::WHITE();

        Maths::mat3D projection = Maths::mat3D::ortho_projection({ -320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f });
        Maths::fvec3 modelTranslation = 0;
        Maths::fvec3 modelScale = 1;
        Maths::fvec3 modelRotation = 0;
    public:
        TestTexturedSquare() {}
        ~TestTexturedSquare() override {}

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override {}
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}
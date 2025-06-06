#pragma once
#include "CameraController2D.h"
#include "Test.h"
#include "Mesh.h"
#include "Physics/World2D.h"

namespace Test {
    class TestCircleCollision2D : public Test {
        static constexpr u32 DYNAMIC_BALL_COUNT = 95,
                             STATIC_BALL_COUNT  = 5,
                             TOTAL_BALL_COUNT   = DYNAMIC_BALL_COUNT + STATIC_BALL_COUNT;

        using Vertex = Graphics::Vertex2D;
        Graphics::RenderObject<Vertex> scene;
        Graphics::Mesh<Vertex> circleMesh;
        Graphics::Shader lineShader;
        Graphics::Mesh<Vertex> totalLineMesh;

        Physics2D::World world;
        OptRef<const Physics2D::Body> edge[4] = { nullptr };

        Graphics::CameraController2D camera;

        OptRef<Physics2D::Body> selected = nullptr;
        Math::fv2 selectOffset = 0;

        DEFINE_TEST_T(TestCircleCollision2D, SIM_PHYSICS);
    public:
        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override;
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;

        void AddRandomBall(Graphics::GraphicsDevice& gdevice);
        void ResetBalls(Graphics::GraphicsDevice& gdevice);
        OptRef<Physics2D::Body> FindBallAt(const Math::fv2& mousePos);
    };
} // Test

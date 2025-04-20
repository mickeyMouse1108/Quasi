#pragma once
#include "Test.h"
#include "Graphicals/Mesh.h"
#include "Physics/World2D.h"

namespace Test {
    class TestPhysicsPlayground2D : public Test {
        using Vertex = Graphics::VertexColor2D;

        struct Object {
            Physics2D::BodyHandle body;
            Math::fColor color;
        };

        Graphics::RenderObject<Vertex> scene;
        Graphics::Mesh<Vertex> worldMesh;
        Vec<Object> bodyData;
        Physics2D::World world;

        Math::fVector2 lastDragPosition;
        Math::fVector2 selectOffset;
        Math::fVector2 controlOffset;
        Math::fVector2 forceAddedPosition;
        u32 controlIndex = ~0;
        u32 selectedIndex = ~0;
        bool hasAddedForce = false;
        bool selectedIsStatic = false;
        Math::fVector2 addedVelocity;

        float zoomFactor = 1.0f;
        Math::fVector2 cameraPosition;

        u32 onPause = 0;

        DEFINE_TEST_T(TestPhysicsPlayground2D, SIM_PHYSICS)
    public:
        TestPhysicsPlayground2D() = default;
        ~TestPhysicsPlayground2D() override = default;

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override;
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;

        void Select(u32 toSelect);
        void Unselect();

        OptRef<Object> Selected();
        void AddNewPoint(const Math::fVector2& point, const Math::fColor& color);
        u32 FindAt(const Math::fVector2& mousePos) const;
        void AddBodyTint(const Math::fColor& color);

        void SelectControl(const Math::fVector2& mouse);
        void SelectControlPoint(const Math::fVector2& mouse, const Math::fVector2& control, u32 i);
        void EditControl(const Math::fVector2& mouse);
        void EditControlPoint(const Math::fVector2& mouse, Math::fVector2& control, u32 i);
        void DrawControlPoints();
        void EditBody();

        void AddRandomCircle (Math::RandomGenerator& rand);
        void AddRandomCapsule(Math::RandomGenerator& rand);
        void AddRandomTri    (Math::RandomGenerator& rand);
        void AddRandomRect   (Math::RandomGenerator& rand);
        void AddRandomQuad   (Math::RandomGenerator& rand);
        void AddRandomPolygon(Math::RandomGenerator& rand);
    };
}

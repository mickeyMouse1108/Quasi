#pragma once
#include "Canvas.h"
#include "Test.h"

namespace Test {
    class DemoUIPlayground : public Test {
        Graphics::Canvas canvas;
        int capMode = 0;
        int joinMode = 0;
        int arcDirection = Graphics::Canvas::CCW;
        int arcMode = Graphics::Canvas::OPEN;

        Math::fv2 bezPoints[3] = { { 200, 700 }, { 600, 1100 }, { 500, 400 } };
    public:
        DemoUIPlayground() = default;
        ~DemoUIPlayground() override = default;

        void OnInit(Graphics::GraphicsDevice& gdevice) override;
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override;
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

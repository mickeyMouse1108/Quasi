#pragma once
#include "Test.h"

namespace Test {
    class TestClearColor : public Test {
    private:
        Maths::Vector4 clearColor = { 0.8f, 0.9f, 1.0f, 1.0f };
    public:
        TestClearColor() {}
        ~TestClearColor() override {}

        OPENGL_API void OnRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        OPENGL_API void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

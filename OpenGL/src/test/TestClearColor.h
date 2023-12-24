#pragma once
#include "Test.h"

namespace Test
{
    class TestClearColor : public Test
    {
    private:
        float clearColor[4];
    public:
        OPENGL_API TestClearColor();
        OPENGL_API ~TestClearColor() override;

        OPENGL_API void OnUpdate(float deltaTime) override;
        OPENGL_API void OnRender(Graphics::Renderer& renderer) override;
        OPENGL_API void OnImGuiRender() override;
    };
}

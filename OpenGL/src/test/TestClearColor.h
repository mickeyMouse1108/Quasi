#pragma once
#include "Test.h"

namespace Test
{
    class TestClearColor : public Test
    {
    private:
        float clearColor[4];
    public:
        TestClearColor();
        ~TestClearColor() override;

        void OnUpdate(float deltaTime) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
    };
}

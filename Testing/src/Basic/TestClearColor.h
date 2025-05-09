﻿#pragma once
#include "Test.h"

namespace Test {
    class TestClearColor : public Test {
    private:
        Math::fColor clearColor = Math::fColor::FromColorID(Math::Colors::BETTER_BLUE);

        DEFINE_TEST_T(TestClearColor, BASIC)
    public:
        TestClearColor() = default;

        void OnInit(Graphics::GraphicsDevice& gdevice) override {}
        void OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) override {}
        void OnRender(Graphics::GraphicsDevice& gdevice) override;
        void OnImGuiRender(Graphics::GraphicsDevice& gdevice) override;
        void OnDestroy(Graphics::GraphicsDevice& gdevice) override;
    };
}

#include "TestClearColor.h"

#include "Debugging.h"
#include <GL/glew.h>

#include "imgui.h"

namespace Test
{
    TestClearColor::TestClearColor() : clearColor{0.8f, 0.9f, 1.0f, 1.0f} {}

    TestClearColor::~TestClearColor()
    {
        GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnUpdate(float deltaTime) { Test::OnUpdate(deltaTime); }

    void TestClearColor::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);
        GLCALL(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender()
    {
        Test::OnImGuiRender();
        {
            ImGui::ColorEdit4("Clear Color", clearColor);
        }
    }
}

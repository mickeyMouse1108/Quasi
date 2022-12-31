#pragma once

#include "imgui.h"
#include "Test.h"
#include "TestClearColor.h"
#include "TestTexturedSquare.h"
#include "TestBatchedColored.h"
#include "TestBatchedTextured.h"
#include "TestDynamicVertexGeometry.h"
#include "TestDynamicQuadGeometry.h"
#include "TestDynamicTris.h"
#include "TestCubeRender.h"

namespace Test
{
    class TestManager : public Test
    {
    private:
        Test* currentTest = nullptr;
        TestMenu* menu = new TestMenu(currentTest);
        bool debugMode = false;
    public:
        TestManager();
        ~TestManager() override;
        
        void OnUpdate(float deltaTime = -1) override;
        void OnRender(Graphics::Renderer& renderer) override;
        void OnImGuiRender() override;
    };

    inline TestManager::TestManager()
    {
        currentTest = menu;
        {
            menu->RegisterTest<TestClearColor>           ("Clear Color Test            ");
            menu->RegisterTest<TestTexturedSquare>       ("Texture Square Test         ");
            menu->RegisterTest<TestBatchedColored>       ("Colored Batching Test       ");
            menu->RegisterTest<TestBatchedTextured>      ("Texture Batching Test       ");
            menu->RegisterTest<TestDynamicVertexGeometry>("Dynamic Vertex Geometry Test");
            menu->RegisterTest<TestDynamicQuadGeometry>  ("Dynamic Quad Geometry Test  ");
            menu->RegisterTest<TestDynamicTris>          ("Dynamic Tris Test           ");
            menu->RegisterTest<TestCubeRender>           ("Cube 3D Rendering Test      ");
        }
    }

    inline TestManager::~TestManager()
    {
        delete currentTest;
        if (currentTest != menu)
            delete menu;
    }

    inline void TestManager::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
        // TODO: make it when dt = -1, auto calc dt
        if (currentTest)
            currentTest->OnUpdate(deltaTime);
    }
    
    inline void TestManager::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);
        if (currentTest)
            currentTest->OnRender(renderer);
    }

    inline void TestManager::OnImGuiRender()
    {
        Test::OnImGuiRender();
        if (currentTest)
        {
            ImGui::Begin("Test");
            if (currentTest != menu && ImGui::Button("<- Back"))
            {
                delete currentTest;
                currentTest = menu;
            }
            
            currentTest->OnImGuiRender();
            
            ImGui::Separator();
            if (ImGui::Button(debugMode ? "Show Debug Menu..." : "Hide Debug Menu...")) debugMode ^= true;

            if (debugMode)
            {
                ImGui::Text("Application Averages %fms/frame (~%f FPS)", 1000.0 / (double)ImGui::GetIO().Framerate, (double)ImGui::GetIO().Framerate);
            }

            ImGui::End();
        }
    }

}

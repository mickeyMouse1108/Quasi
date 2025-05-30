#include <imgui.h>

#include "Debug/Logger.h"

#include "TestManager.h"

#include "Basic/TestClearColor.h"
#include "Basic/TestBatchedTextured.h"
#include "Basic/TestDynamicVertexGeometry.h"
#include "Basic/TestDynamicQuadGeometry.h"
#include "Basic/TestCubeRender.h"

#include "Advanced/TestFontRender.h"
#include "Advanced/TestPostProcessing.h"
#include "Advanced/TestAdvancedLighting.h"
#include "Advanced/TestBezierCurves.h"
#include "Advanced/TestMaterialMaps.h"
#include "Advanced/TestLightCasters.h"
#include "Advanced/TestCubeMap.h"
#include "Advanced/TestDrawInstances.h"
#include "Advanced/TestGeometryShader.h"
#include "Advanced/TestShadowMap.h"

#include "Demos/DemoFlappyBird.h"

#include "Physics/TestCircleCollision2D.h"
#include "Physics/TestPhysicsPlayground2D.h"

namespace Test {
    void TestManager::OnInit() {
        menu = Box<TestMenu>::Build(*this);
        currentTest = *menu;
        {
            menu->DeclareTestType(TestType::BASIC);
            menu->AddSectionDescription("Simple OpenGL Tests to make sure the renderer is working. \nTests mostly involve: Texture Rendering and Vertex manipulation");

            menu->RegisterTest<TestClearColor>("Clear Color");
            menu->AddDescription("Clears the screen with a color.");

            menu->RegisterTest<TestBatchedTextured>("Texture Batching");
            menu->AddDescription("Draws 2 textured squares.");

            menu->RegisterTest<TestDynamicVertexGeometry>("Dyn Vertex Geometry");
            menu->AddDescription("Draws 2 Modifiable squares.");

            menu->RegisterTest<TestDynamicQuadGeometry>("Dyn Quad Geometry");
            menu->AddDescription("Draws up to 8 Unique Modifiable squares.");

            menu->RegisterTest<TestCubeRender>("Cube 3D Rendering");
            menu->AddDescription("Draws a 3D cube.");

            // =========================================================================

            menu->DeclareTestType(TestType::ADVANCED);
            menu->AddSectionDescription("Advanced OpenGL Tests focused on more realistic rendering. \nIncluding: Text Rendering, Post Processing Effects and Lighting");

            menu->RegisterTest<TestFontRender>("Font Rendering");
            menu->AddDescription("Draws text (not 100% finished yet: markdown).");

            menu->RegisterTest<TestPostProcessing>("Post Processing");
            menu->AddDescription("Draws a simple scene with post processing.");

            menu->RegisterTest<TestAdvancedLighting>("Advanced Lighting");
            menu->AddDescription("Draws a simple scene with lighting.");

            menu->RegisterTest<TestMaterialMaps>("Material Map");
            menu->AddDescription("Draws materials with texture maps.");

            menu->RegisterTest<TestLightCasters>("Light Casters");
            menu->AddDescription("Draws a scene with many types of light casting.");

            menu->RegisterTest<TestCubeMap>("Cube Map");
            menu->AddDescription("Draws a skybox using a cubemap texture.");

            menu->RegisterTest<TestGeometryShader>("Geometry Shader");
            menu->AddDescription("Draws spheres using geometry shaders.");

            menu->RegisterTest<TestDrawInstances>("Draw Instanced");
            menu->AddDescription("Draws cubes using instancing.");

            menu->RegisterTest<TestShadowMap>("Shadow Map");
            menu->AddDescription("Draws a scene with simple shadows.");

            menu->RegisterTest<TestBezierCurves>("Bezier Curves");
            menu->AddDescription("Draws resolution-independent bezier curves.");
            // =========================================================================

            menu->DeclareTestType(TestType::SIM_PHYSICS);
            menu->AddSectionDescription("Physics Simulations designed to test Physics implementations.");

            menu->RegisterTest<TestCircleCollision2D>("2D Circle Collisions");
            menu->AddDescription("Checks for Collisions between circles.");

            menu->RegisterTest<TestPhysicsPlayground2D>("2D Physics Playground");
            menu->AddDescription("A physics sandbox in 2D");

            // =========================================================================

            menu->DeclareTestType(TestType::DEMO);
            menu->AddSectionDescription("Simple OpenGL 'Games' created to explore the limitations of the engine. \nJust to see what's possible without over-development.");

            menu->RegisterTest<DemoFlappyBird>("Flappy Bird");
            menu->AddDescription("A dumb-down version of Flappy Bird.");

            // =========================================================================

            menu->DeclareTestType(TestType::OTHER);
            menu->AddSectionDescription("Other Tests that didn't fit in. \nThere's nothing here yet!");

            menu->FinishDeclare();
        }

        {
            const CStr fontFile = Q_USER_FONTS "JetBrainsMono-Regular.ttf";
            if (!Text::ExistsFile(fontFile)) {
                Debug::QWarn$("couldn't load font {}, some characters may not load properly", fontFile);
                return;
            }

            const ImGuiIO& io = ImGui::GetIO();

            ImVector<ImWchar> glyphRange;
            ImFontGlyphRangesBuilder builder;
            builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
            builder.AddText((const char*)u8"°αβγνξΔδεζπΠρΘΛθιητυκλμΨΣςσΦφΩψω"); // greek letters
            builder.BuildRanges(&glyphRange);

            io.Fonts->AddFontFromFileTTF(fontFile.Data(), 21, nullptr, glyphRange.Data);
            io.Fonts->Build();
        }
    }

    void TestManager::OnUpdate(float deltaTime) {
        deltaTime = std::isnan(deltaTime) ? gdevice.GetIO().Time.DeltaTime() : deltaTime;
        currentTest->OnUpdate(gdevice, deltaTime);
    }

    void TestManager::OnRender() {
        currentTest->OnRender(gdevice);
    }

    void TestManager::OnImGuiRender() {
        ImGui::Begin("Test");
        if (currentTest.RefEquals(*testInstance) && ImGui::Button("<< Back")) {
            currentTest->OnDestroy(gdevice);
            testInstance.Close();
            currentTest = *menu;
            currentTest->OnInit(gdevice);
        }

        currentTest->OnImGuiRender(gdevice);

        ImGui::Separator();
        gdevice.DebugMenu();

        ImGui::End();
    }

    void TestManager::OnDestroy() {}

    void TestManager::OnRun() {
        gdevice.Begin();
        OnUpdate();
        OnRender();
        OnImGuiRender();
        gdevice.End();
    }
}

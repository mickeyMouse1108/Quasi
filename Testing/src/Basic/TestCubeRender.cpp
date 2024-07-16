﻿#include "TestCubeRender.h"

#include "imgui.h"
#include "Extension/ImGuiExt.h"
#include "Meshes/Cube.h"

namespace Test {
    void TestCubeRender::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<Graphics::VertexColor3D>(4 * 6, 12);

        using namespace Math;

        using namespace Graphics::VertexBuilder;
        u32 i = 0 - 1;
        cube = Graphics::MeshUtils::Cube(Graphics::VertexColor3D::Blueprint {
            .Position = GetPosition {},
            .Color = FromArg<>([&] { ++i; return fColor::color_id((int)(i / 4) + 1); })
        });

        render.UseShaderFromFile(res("shader.vert"), res("shader.frag"));
        render.SetProjection(projection);
    }

    void TestCubeRender::OnRender(Graphics::GraphicsDevice& gdevice) {
        render.Draw(cube, Graphics::UseArgs({{ "u_alpha", alpha }}));
    }

    void TestCubeRender::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        ImGui::EditTransform("Transform", cube.modelTransform, 0.01f);
        ImGui::EditScalar("Transparency", alpha, 0.01f, Math::fRange { 0, 1 });
    }

    void TestCubeRender::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        render.Destroy();
    }
}

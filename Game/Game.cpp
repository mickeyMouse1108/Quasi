#include <iostream>
#include "Graphics/Graphicals/GraphicsDevice.h"
#include "test/TestingFramework.h"
#include "Mesh.h"
#include "imgui.h"

int main() {
    std::unique_ptr<Graphics::GraphicsDevice> GraphicsDevice = Graphics::GraphicsDevice::Initialize();
    /*Test::TestManager testm;

    while (GraphicsDevice->WindowIsOpen()) {
        testm.OnUpdate();

        GraphicsDevice->BeginRender();

        testm.OnImGuiRender();
        testm.OnRender(GraphicsDevice->GetRenderer());

        GraphicsDevice->EndRender();
    }*/

    Maths::Vector3 trans = { 0, 0, -2 }, scale = {1, 1, 1}, rot;
    Graphics::Mesh<> cube = Graphics::MeshUtils::CubeMesh(0, 1, 1, 1);
    cube.Bind(*GraphicsDevice);

    GraphicsDevice->SetProjection(Maths::Matrix3D::PerspectiveProjectionFOV(90.0f, 4.0f / 3, 0.1, 100));
    GraphicsDevice->UseShaderFromFile("basic_tex.glsl");
    // GraphicsDevice->RegisterMeshes();
    /* Loop until the user closes the window */
    while (GraphicsDevice->WindowIsOpen()) {
        GraphicsDevice->BeginRender();

        GraphicsDevice->ClearRegistered();
        GraphicsDevice->RegisterMeshes();
        GraphicsDevice->EnableShader();
        GraphicsDevice->RenderRegistered();

        Maths::Matrix3D mat = Maths::Matrix3D::Transform(trans, scale, rot);
        GraphicsDevice->SetCamera(mat);
        ImGui::SliderFloat3("Rot", (float*)&rot, -3.14f, 3.14f);
        ImGui::SliderFloat3("Sca", (float*)&scale, 0, 3);
        ImGui::SliderFloat3("Tra", (float*)&trans, -10, 10);

        GraphicsDevice->DebugMenu();

        GraphicsDevice->EndRender();
    }
}

#include "TestDynamicVertexGeometry.h"

#include "imgui.h"

namespace Test {
    void TestDynamicVertexGeometry::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<VertexColor3D>(8, 4);

        render->UseShader(Graphics::Shader::StdColored);
        render->SetProjection(projection);

        VertexColor3D vertices[] = {
            { { -240.0f, -80.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
            { { -80.00f, -80.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
            { { -80.00f, +80.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
            { { -240.0f, +80.0f, 0.0f }, { 0.5f, 0.5f, 0.5f } },
            
            { { +80.00f, -80.0f, 0.0f }, { 1.0f, 0.0f, 1.0f } },
            { { +240.0f, -80.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
            { { +240.0f, +80.0f, 0.0f }, { 0.0f, 1.0f, 1.0f } },
            { { +80.00f, +80.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
        };

        Graphics::TriIndices indices[4] = {
            { 0, 1, 2 },  { 2, 3, 0 },
            { 4, 5, 6 },  { 6, 7, 4 }
        };

        mesh = Graphics::Mesh(
            std::vector(vertices, vertices + 8),
            std::vector(indices, indices + 4)
        );

        render->BindMeshes(&mesh, 1);
    }

    void TestDynamicVertexGeometry::OnRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnRender(gdevice);

        render->ResetData<VertexColor3D>();
        render->Render();
    }

    void TestDynamicVertexGeometry::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnImGuiRender(gdevice);

        VertexColor3D* vertices = mesh.GetVertices().data();
        ImGui::DragFloat3("Red    Vertex [0]", vertices[0].Position);
        ImGui::DragFloat3("Green  Vertex [1]", vertices[1].Position);
        ImGui::DragFloat3("Blue   Vertex [2]", vertices[2].Position);
        ImGui::DragFloat3("Gray   Vertex [3]", vertices[3].Position);

        ImGui::DragFloat3("Purple Vertex [4]", vertices[4].Position);
        ImGui::DragFloat3("Yellow Vertex [5]", vertices[5].Position);
        ImGui::DragFloat3("Cyan   Vertex [6]", vertices[6].Position);
        ImGui::DragFloat3("White  Vertex [7]", vertices[7].Position);
    }

    void TestDynamicVertexGeometry::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        Test::OnDestroy(gdevice);
        render->Destroy();
    }
}

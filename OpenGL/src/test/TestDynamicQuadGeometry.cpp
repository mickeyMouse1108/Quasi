#include "TestDynamicQuadGeometry.h"
#include "Quad.h"

#include "imgui.h"

namespace Test {
    Maths::Vector4 TestDynamicQuadGeometry::COLORS[8] = {
        { 1.0f, 0.0f, 0.0f, 1.0f }, // red
        { 0.0f, 1.0f, 0.0f, 1.0f }, // green
        { 0.0f, 0.0f, 1.0f, 1.0f }, // blue
        { 1.0f, 1.0f, 0.0f, 1.0f }, // magenta
        { 0.0f, 1.0f, 1.0f, 1.0f }, // yellow
        { 1.0f, 0.0f, 1.0f, 1.0f }, // cyan
        { 1.0f, 0.5f, 0.0f, 1.0f }, // orange 
        { 0.5f, 0.0f, 1.0f, 1.0f }, // purple
    };
    
    void TestDynamicQuadGeometry::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<VertexColor3D>(8 * 4, 8 * 2);

        gdevice.UseShader(Graphics::Shader::StdColored);
        gdevice.SetProjection(projection);

        quads.push_back(NewQuad());
        quads.back().Bind(*render);
    }

    void TestDynamicQuadGeometry::OnRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnRender(gdevice);

        render->ResetData<VertexColor3D>();
        render->Render();
    }

    void TestDynamicQuadGeometry::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnImGuiRender(gdevice);

        uint quadCount = quads.size();

        auto& verts = quads.back().GetVerticies();
        ImGui::DragFloat3("Quad Vertex 1", verts[0].Position);
        ImGui::DragFloat3("Quad Vertex 2", verts[1].Position);
        ImGui::DragFloat3("Quad Vertex 3", verts[2].Position);
        ImGui::DragFloat3("Quad Vertex 4", verts[3].Position);

        if (ImGui::Button("Add Quad") && !isMax) {
            if (quadCount >= MAX_QUAD) { isMax = true; } else {
                quads.push_back(NewQuad());
                quads.back().Bind(*render);

                isMin = false;
            }
        }
        
        if (isMax) ImGui::Text("Too Many Quads! (Limit: %i)", MAX_QUAD); 
        
        if (ImGui::Button("Remove Quad") && !isMin) {
            if (quadCount <= 1) { isMin = true; } else {
                quads.back().Unbind();
                quads.pop_back();
                isMax = false;
            }
        }

        if (isMin) ImGui::Text("Can't Delete All Quads!");
    }

    void TestDynamicQuadGeometry::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        Test::OnDestroy(gdevice);
        render->Destroy();
    }

    Graphics::Mesh<VertexColor3D> TestDynamicQuadGeometry::NewQuad() {
        Graphics::Primitives::Quad quad = { 0, { 50, 0 }, { 0, 50 } };
        return quad.IntoMesh<VertexColor3D>()
                   .ApplyMaterial(&VertexColor3D::Color, COLORS[quads.size()]);
    }
}

#include "TestDynamicTris.h"
#include "Tri.h"

#include "imgui.h"

namespace Test {
    Maths::fvec4 TestDynamicTris::COLORS[8] = {
        { 1.0f, 0.0f, 0.0f, 1.0f }, // red
        { 0.0f, 1.0f, 0.0f, 1.0f }, // green
        { 0.0f, 0.0f, 1.0f, 1.0f }, // blue
        { 1.0f, 1.0f, 0.0f, 1.0f }, // magenta
        { 0.0f, 1.0f, 1.0f, 1.0f }, // yellow
        { 1.0f, 0.0f, 1.0f, 1.0f }, // cyan
        { 1.0f, 0.5f, 0.0f, 1.0f }, // orange 
        { 0.5f, 0.0f, 1.0f, 1.0f }, // purple
    };

    Maths::mat3D TestDynamicTris::ModelMatrix() {
        return Maths::mat3D::transform({ modelTranslation, 0.0f }, { modelScale, 1.0f }, { 0.0f, 0.0f, modelRotation });
    }

    void TestDynamicTris::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<VertexColor3D>(3 * 8, 8);

        render->UseShader(Graphics::Shader::StdColored);
        render->SetProjection(projection);
        
        tris.push_back(NewTri());
        tris.back().Bind(*render);
    }

    void TestDynamicTris::OnRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnRender(gdevice);

        tris.back().SetTransform(ModelMatrix());
        
        render->ResetData<VertexColor3D>();
        render->Render();
    }

    void TestDynamicTris::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnImGuiRender(gdevice);

        ImGui::DragFloat2("Current Tri Translation", modelTranslation       );
        ImGui::DragFloat2("Current Tri Scale      ", modelScale      , 0.10f);
        ImGui::DragFloat ("Current Tri Rotation   ", &modelRotation  , 0.05f);

        uint triCount = tris.size();

        if (ImGui::Button("Add Tri") && !isMax) {
            if (triCount >= MAX_TRIS) { isMax = true; } else {
                tris.push_back(NewTri());
                tris.back().Bind(*render);
                // Graphics::Mesh(vc3, { { 0, 1, 2 } });
                modelTranslation = 0;
                modelScale = 1;
                modelRotation = 0.0f;
                
                isMin = false;
            }
        }
        
        if (isMax) ImGui::Text("Too Many Tris! (Limit: %i)", MAX_TRIS); 
        
        if (ImGui::Button("Remove Tri") && !isMin) {
            if (triCount <= 1) { isMin = true; } else {
                tris.back().Unbind();
                tris.pop_back();
                modelTranslation = 0;
                modelScale = 1;
                modelRotation = 0.0f;
                
                isMax = false;
            }
        }

        if (isMin) ImGui::Text("Can't Delete All Tris!");
    }

    void TestDynamicTris::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        Test::OnDestroy(gdevice);
        render->Destroy();
    }

    Graphics::Mesh<VertexColor3D> TestDynamicTris::NewTri() {
        return Graphics::Primitives::Tri(
            { +00.0f, +50.0f, 0.0f },
            { -50.0f, -50.0f, 0.0f },
            { +50.0f, -50.0f, 0.0f }
        ).IntoMesh<VertexColor3D>()
         .ApplyMaterial(&VertexColor3D::Color, COLORS[tris.size()]);
    }
}

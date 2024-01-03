#include "TestBatchedColored.h"
#include "VertexElement.h"
#include "TriIndicies.h"

#include "imgui.h"

namespace Test {
    void TestBatchedColored::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<VertexColor3D>(8, 4);
        VertexColor3D vertices[4 * 2] = {
            { { -240.0f, -80.0f }, { 0.1255f, 0.2000f, 0.8588f, 1 } },
            { { -80.00f, -80.0f }, { 0.3647f, 0.2000f, 0.9000f, 1 } },
            { { -80.00f, +80.0f }, { 0.1255f, 0.2000f, 0.8588f, 1 } },
            { { -240.0f, +80.0f }, { 0.1608f, 0.4000f, 0.8059f, 1 } },

            { { +240.0f, -80.0f }, { 0.9020f, 0.5451f, 0.0431f, 1 } },
            { { +80.00f, -80.0f }, { 0.9490f, 0.7451f, 0.0745f, 1 } },
            { { +80.00f, +80.0f }, { 0.9686f, 0.9137f, 0.1176f, 1 } },
            { { +240.0f, +80.0f }, { 0.9490f, 0.7451f, 0.0745f, 1 } },
        };

        Graphics::TriIndicies indices[4] = {
            { 0, 1, 2 },  { 2, 3, 0 },
            { 4, 5, 6 },  { 6, 7, 4 }
        };
        mesh = Graphics::Mesh<VertexColor3D> {
            std::vector(vertices, vertices + 8),
            std::vector(indices,  indices  + 4)
        };

        render->BindMeshes(&mesh, 1);

        gdevice.UseShader(Graphics::Shader::StdColored);
        gdevice.SetProjection(projection);
    }

    void TestBatchedColored::OnRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnRender(gdevice);
        //shader.SetUniform4F("u_Color", color);
        Maths::Matrix3D mat = Maths::Matrix3D::Transform(modelTranslation,
                                                         modelScale,
                                                         modelRotation);
        gdevice.SetCamera(mat);

        //LOG(mat);
        render->ResetData<VertexColor3D>();
        render->Render();
    }

    void TestBatchedColored::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnImGuiRender(gdevice);
        
        ImGui::DragFloat3("Translation", modelTranslation);
        ImGui::DragFloat3("Scale",       modelScale      , 0.1f);
        ImGui::DragFloat3("Rotation",    modelRotation   , 0.03f);
    }

    void TestBatchedColored::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        Test::OnDestroy(gdevice);
        render->Destroy();
    }
}

#include "TestCubeRender.h"
#include "Quad.h"

#include <algorithm>

#include "imgui.h"

namespace Test {
    void TestCubeRender::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<VertexColor3D>(4 * 6, 12);

        using Graphics::Primitives::Quad;

        cube = Graphics::Mesh<VertexColor3D>::Combine<6>(std::array {
                Quad({ +1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 })
                    .IntoMesh<VertexColor3D>()
                    .ApplyMaterial(&VertexColor3D::Color, { 1, 0, 0, 1 }),
                Quad({ -1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 })
                    .IntoMesh<VertexColor3D>()
                    .ApplyMaterial(&VertexColor3D::Color, { 0, 1, 1, 1 }),
                Quad({ 0, +1, 0 }, { 1, 0, 0 }, { 0, 0, 1 })
                    .IntoMesh<VertexColor3D>()
                    .ApplyMaterial(&VertexColor3D::Color, { 0, 1, 0, 1 }),
                Quad({ 0, -1, 0 }, { 1, 0, 0 }, { 0, 0, 1 })
                    .IntoMesh<VertexColor3D>()
                    .ApplyMaterial(&VertexColor3D::Color, { 1, 0, 1, 1 }),
                Quad({ 0, 0, +1 }, { 1, 0, 0 }, { 0, 1, 0 })
                    .IntoMesh<VertexColor3D>()
                    .ApplyMaterial(&VertexColor3D::Color, { 0, 0, 1, 1 }),
                Quad({ 0, 0, -1 }, { 1, 0, 0 }, { 0, 1, 0 })
                    .IntoMesh<VertexColor3D>()
                    .ApplyMaterial(&VertexColor3D::Color, { 1, 1, 0, 1 }),
                }
            );

        render->BindMeshes(&cube, 1);

        gdevice.UseShader(
            "#shader vertex\n"
            "#version 330 core\n"
            "layout(location = 0) in vec4 position;\n"
            "layout(location = 1) in vec4 color;\n"
            "out vec4 v_color;\n"
            "out float v_alpha;\n"
            "uniform mat4 u_projection;\n"
            "uniform mat4 u_view;\n"
            "uniform float u_alpha;\n"
            "void main(){\n"
            "    gl_Position = u_projection * u_view * position;\n"
            "    v_color = color;\n"
            "    v_alpha = u_alpha;\n"
            "}\n"
            "#shader fragment\n"
            "#version 330 core\n"
            "layout(location = 0) out vec4 color;\n"
            "in vec4 v_color;\n"
            "in float v_alpha;\n"
            "void main(){\n"
            "    color = v_color;\n"
            "    color.a = v_alpha;\n"
            "}"
        );
        gdevice.SetProjection(projection);
    }

    void TestCubeRender::OnRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnRender(gdevice);

        Maths::Matrix3D mat = Maths::Matrix3D::Transform(modelTranslation, modelScale, modelRotation);
        
        // std::sort(faceOrder, faceOrder + 6,
        // [&](unsigned int face1, unsigned int face2){ return (mat * faceAxis[face1]).z < (mat * faceAxis[face2]).z; });
        gdevice.SetCamera(mat);
        gdevice.GetShader().Bind();
        gdevice.GetShader().SetUniform1F("u_alpha", alpha);

        render->ResetData<VertexColor3D>();
        render->Render();
    }

    void TestCubeRender::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnImGuiRender(gdevice);

        ImGui::DragFloat3("Translation" , modelTranslation, 0.01f);
        ImGui::DragFloat3("Scale"       , modelScale      , 0.01f);
        ImGui::DragFloat3("Rotation"    , modelRotation   , 0.01f);
        ImGui::DragFloat ("Transparency", &alpha          , 0.01f);
    }

    void TestCubeRender::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        Test::OnDestroy(gdevice);
        render->Destroy();
    }
}

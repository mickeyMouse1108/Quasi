#include "TestCubeRender.h"
#include "Quad.h"

#include <algorithm>

#include "imgui.h"

namespace Test {
    // Maths::Vector3 TestCubeRender::faceAxis[6] = {
    //     { +1.0f, +0.0f, +0.0f },
    //     { -1.0f, +0.0f, +0.0f },
    //     { +0.0f, +1.0f, +0.0f },
    //     { +0.0f, -1.0f, +0.0f },
    //     { +0.0f, +0.0f, +1.0f },
    //     { +0.0f, +0.0f, -1.0f },
    // };
    
    TestCubeRender::TestCubeRender()
    : projection(Maths::Matrix3D::OrthoProjection(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f)) {
        va = new Graphics::VertexArray();
        vb = new Graphics::DynamicVertexBuffer<VertexColor3D>(4 * 6);
        
        va->AddBuffer(*vb);

        ib = new Graphics::DynamicIndexBuffer(6 * 6);
        shader = new Graphics::Shader(
            "#shader vertex\n"
            "#version 330 core\n"
            "layout(location = 0) in vec4 position;\n"
            "layout(location = 1) in vec4 color;\n"
            "out vec4 v_color;\n"
            "out float v_alpha;\n"
            "uniform mat4 u_MVP;\n"
            "uniform float u_alpha;\n"
            "void main(){\n"
            "    gl_Position = u_MVP * position;\n"
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
            // Graphics::Shader::StdColored
        );
        shader->Bind();

        shader->SetUniformMatrix4x4("u_MVP", projection);
        // shader->SetUniform1F("u_alpha", alpha);
        
        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        using Graphics::Primitives::Quad;

        auto face0 = Quad({ +1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 })
            .IntoMesh<VertexColor3D>();
        face0.ApplyMaterial(&VertexColor3D::Color, { 1, 0, 0, 1 });

        auto face1 = Quad({ -1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 })
            .IntoMesh<VertexColor3D>();
        face1.ApplyMaterial(&VertexColor3D::Color, { 0, 1, 1, 1 });

        auto face2 = Quad({ 0, +1, 0 }, { 1, 0, 0 }, { 0, 0, 1 })
            .IntoMesh<VertexColor3D>();
        face2.ApplyMaterial(&VertexColor3D::Color, { 0, 1, 0, 1 });

        auto face3 = Quad({ 0, -1, 0 }, { 1, 0, 0 }, { 0, 0, 1 })
            .IntoMesh<VertexColor3D>();
        face3.ApplyMaterial(&VertexColor3D::Color, { 1, 0, 1, 1 });

        auto face4 = Quad({ 0, 0, +1 }, { 1, 0, 0 }, { 0, 1, 0 })
            .IntoMesh<VertexColor3D>();
        face4.ApplyMaterial(&VertexColor3D::Color, { 0, 0, 1, 1 });

        auto face5 = Quad({ 0, 0, -1 }, { 1, 0, 0 }, { 0, 1, 0 })
            .IntoMesh<VertexColor3D>();
        face5.ApplyMaterial(&VertexColor3D::Color, { 1, 1, 0, 1 });

        cube = std::move(face0);
        cube.Add(face1);
        cube.Add(face2);
        cube.Add(face3);
        cube.Add(face4);
        cube.Add(face5);

        cube.AddTo(*vb, *ib);
    }

    TestCubeRender::~TestCubeRender() {
        delete va;
        delete vb;
        delete ib;
        delete shader;
    }

    void TestCubeRender::OnUpdate(float deltaTime) {
        Test::OnUpdate(deltaTime);
    }

    void TestCubeRender::OnRender(Graphics::Renderer& renderer) {
        Test::OnRender(renderer);

        Maths::Matrix3D mat = Maths::Matrix3D::Transform(modelTranslation, modelScale, modelRotation);
        
        // std::sort(faceOrder, faceOrder + 6,
        // [&](unsigned int face1, unsigned int face2){ return (mat * faceAxis[face1]).z < (mat * faceAxis[face2]).z; });
        vb->ClearData();
        ib->ClearData();
        cube.AddTo(*vb, *ib);

        shader->Bind();
        shader->SetUniformMatrix4x4("u_MVP", projection * mat);
        shader->SetUniform1F("u_alpha", alpha);
        shader->Unbind();
        
        renderer.Draw(*va, *ib, *shader);
    }

    void TestCubeRender::OnImGuiRender()
    {
        Test::OnImGuiRender();

        ImGui::DragFloat3("Translation" , modelTranslation, 0.01f);
        ImGui::DragFloat3("Scale"       , modelScale      , 0.01f);
        ImGui::DragFloat3("Rotation"    , modelRotation   , 0.01f);
        ImGui::DragFloat ("Transparency", &alpha          , 0.01f);
    }
}

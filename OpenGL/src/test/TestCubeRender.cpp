#include "TestCubeRender.h"

#include <algorithm>

#include "imgui.h"

namespace Test
{
    // Maths::Vector3 TestCubeRender::faceAxis[6] = {
    //     { +1.0f, +0.0f, +0.0f },
    //     { -1.0f, +0.0f, +0.0f },
    //     { +0.0f, +1.0f, +0.0f },
    //     { +0.0f, -1.0f, +0.0f },
    //     { +0.0f, +0.0f, +1.0f },
    //     { +0.0f, +0.0f, -1.0f },
    // };
    
    TestCubeRender::TestCubeRender()
    : projection(Maths::Matrix3D::OrthoProjection(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f))
    {
        va = new Graphics::VertexArray();
        vb = new Graphics::DynamicVertexBuffer<VertexColor3D>(4 * 6);
        
        va->AddBuffer(*vb);

        ib = new Graphics::DynamicIndexBuffer(6 * 6);
        shader = new Graphics::Shader("src/test/res/TestCubeRender/shader.glsl");
        shader->Bind();

        shader->SetUniformMatrix4x4("u_MVP", projection);
        shader->SetUniform1F("u_alpha", alpha);
        
        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        Maths::Vector3 cubeVertices[8] =
        {
            { +0.5f, +0.5f, +0.5f }, // +++
            { +0.5f, +0.5f, -0.5f }, // ++-
            { +0.5f, -0.5f, +0.5f }, // +-+
            { +0.5f, -0.5f, -0.5f }, // +--
            { -0.5f, +0.5f, +0.5f }, // -++
            { -0.5f, +0.5f, -0.5f }, // -+-
            { -0.5f, -0.5f, +0.5f }, // --+
            { -0.5f, -0.5f, -0.5f }, // ---
        };

#define V VertexColor3D
        
        {
            cubeFaces[0] = MESH_QUAD_CREATE(VertexColor3D,  // left: red
                V { cubeVertices[0], { 1.0f, 0.0f, 0.0f, 1.0f } },
                V { cubeVertices[1], { 1.0f, 0.0f, 0.0f, 1.0f } },
                V { cubeVertices[2], { 1.0f, 0.0f, 0.0f, 1.0f } },
                V { cubeVertices[3], { 1.0f, 0.0f, 0.0f, 1.0f } },
            );

            cubeFaces[1] = MESH_QUAD_CREATE(VertexColor3D,  // right: cyan
                V { cubeVertices[4], { 0.0f, 1.0f, 1.0f, 1.0f } },
                V { cubeVertices[5], { 0.0f, 1.0f, 1.0f, 1.0f } },
                V { cubeVertices[6], { 0.0f, 1.0f, 1.0f, 1.0f } },
                V { cubeVertices[7], { 0.0f, 1.0f, 1.0f, 1.0f } },
            );

            cubeFaces[2] = MESH_QUAD_CREATE(VertexColor3D,  // up: green
                V { cubeVertices[0], { 0.0f, 1.0f, 0.0f, 1.0f } },
                V { cubeVertices[1], { 0.0f, 1.0f, 0.0f, 1.0f } },
                V { cubeVertices[4], { 0.0f, 1.0f, 0.0f, 1.0f } },
                V { cubeVertices[5], { 0.0f, 1.0f, 0.0f, 1.0f } },
            );

            cubeFaces[3] = MESH_QUAD_CREATE(VertexColor3D,  // down: purp
                V { cubeVertices[2], { 1.0f, 0.0f, 1.0f, 1.0f } },
                V { cubeVertices[3], { 1.0f, 0.0f, 1.0f, 1.0f } },
                V { cubeVertices[6], { 1.0f, 0.0f, 1.0f, 1.0f } },
                V { cubeVertices[7], { 1.0f, 0.0f, 1.0f, 1.0f } },
            );

            cubeFaces[4] = MESH_QUAD_CREATE(VertexColor3D,  // back: blue
                V { cubeVertices[0], { 0.0f, 0.0f, 1.0f, 1.0f } },
                V { cubeVertices[2], { 0.0f, 0.0f, 1.0f, 1.0f } },
                V { cubeVertices[4], { 0.0f, 0.0f, 1.0f, 1.0f } },
                V { cubeVertices[6], { 0.0f, 0.0f, 1.0f, 1.0f } },
            );

            cubeFaces[5] = MESH_QUAD_CREATE(VertexColor3D,  // front: yellow
                V { cubeVertices[1], { 1.0f, 1.0f, 0.0f, 1.0f } },
                V { cubeVertices[3], { 1.0f, 1.0f, 0.0f, 1.0f } },
                V { cubeVertices[5], { 1.0f, 1.0f, 0.0f, 1.0f } },
                V { cubeVertices[7], { 1.0f, 1.0f, 0.0f, 1.0f } },
            );
        }
#undef V

        for (int i = 0; i < 6; ++i) cubeFaces[i].AddTo(*vb, *ib);
    }

    TestCubeRender::~TestCubeRender()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
    }

    void TestCubeRender::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestCubeRender::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);

        Maths::Matrix3D mat = Maths::Matrix3D::Transform(modelTranslation, modelScale, modelRotation);
        
        // std::sort(faceOrder, faceOrder + 6,
        // [&](unsigned int face1, unsigned int face2){ return (mat * faceAxis[face1]).z < (mat * faceAxis[face2]).z; });

        vb->ClearData();
        ib->ClearData();
        for (int i = 0; i < 6; ++i) cubeFaces[i].AddTo(*vb, *ib);

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

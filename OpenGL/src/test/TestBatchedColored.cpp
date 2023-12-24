#include "TestBatchedColored.h"

#include "imgui.h"

namespace Test
{
    TestBatchedColored::TestBatchedColored()
    {
        float vertices[(2 + 3) * 4 * 2] =
        {   -240.0f, -80.0f,    0.1255f, 0.2000f, 0.8588f,
            -80.00f, -80.0f,    0.3647f, 0.2000f, 0.9000f,
            -80.00f, +80.0f,    0.1255f, 0.2000f, 0.8588f,
            -240.0f, +80.0f,    0.1608f, 0.4000f, 0.8059f,
      
            +240.0f, -80.0f,    0.9020f, 0.5451f, 0.0431f,
            +80.00f, -80.0f,    0.9490f, 0.7451f, 0.0745f,
            +80.00f, +80.0f,    0.9686f, 0.9137f, 0.1176f,
            +240.0f, +80.0f,    0.9490f, 0.7451f, 0.0745f,
        };
        
        unsigned int indices[6 * 2] = {
            0, 1, 2,  2, 3, 0,
            4, 5, 6,  6, 7, 4
        };
        
        va = new Graphics::VertexArray();
        vb = new Graphics::VertexBuffer(vertices, (2 + 3) * 4 * 2 * sizeof(float));

        Graphics::VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(3);
        va->AddBuffer(*vb, layout);

        ib = new Graphics::IndexBuffer(indices, 12);

        projection = Maths::Matrix3D::OrthoProjection(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        //LOG(*mat);

        shader = new Graphics::Shader(Graphics::Shader::StdColored);
        shader->Bind();
        
        shader->SetUniformMatrix4x4("u_MVP", projection);

        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        modelTranslation = {0.0f, 0.0f, 0.0f};
        modelScale       = {1.0f, 1.0f, 1.0f};
        modelRotation    = {0.0f, 0.0f, 0.0f};
    }

    TestBatchedColored::~TestBatchedColored()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
    }

    void TestBatchedColored::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestBatchedColored::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);
        shader->Bind();
        //shader.SetUniform4F("u_Color", color);
        Maths::Matrix3D mat = Maths::Matrix3D::Transform(modelTranslation,
                                                         modelScale,
                                                         modelRotation);
        //LOG(mat);
        shader->SetUniformMatrix4x4("u_MVP", projection * mat);
        shader->Unbind();
        
        renderer.Draw(*va, *ib, *shader);
    }

    void TestBatchedColored::OnImGuiRender()
    {
        Test::OnImGuiRender();
        
        ImGui::DragFloat3("Translation", modelTranslation);
        ImGui::DragFloat3("Scale",       modelScale      , 0.1f);
        ImGui::DragFloat3("Rotation",    modelRotation   , 0.03f);
    }
}

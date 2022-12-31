#include "TestTexturedSquare.h"

#include "imgui.h"

namespace Test
{
    TestTexturedSquare::TestTexturedSquare()
    {
        float vertices[16] =
        {   -50.0f, -50.0f, 0.0f, 0.0f,
            +50.0f, -50.0f, 1.0f, 0.0f,
            +50.0f, +50.0f, 1.0f, 1.0f,
            -50.0f, +50.0f, 0.0f, 1.0f
        };

        unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };
        
        va = new Graphics::VertexArray();
        vb = new Graphics::VertexBuffer(vertices, 4 * 4 * sizeof(float));

        Graphics::VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va->AddBuffer(*vb, layout);

        ib = new Graphics::IndexBuffer(indices, 6);

        projection = Maths::Matrix3D::OrthoProjection(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        //LOG(*mat);

        shader = new Graphics::Shader("src/test/res/TestTexturedSquare/shader.glsl");
        shader->Bind();

        texture = new Graphics::Texture("res/textures/UV.png");
        texture->Bind();
        shader->SetUniform1I("u_Texture", 0);
        shader->SetUniformMatrix4x4("u_MVP", projection);

        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        modelTranslation = {0.0f, 0.0f, 0.0f};
        modelScale       = {1.0f, 1.0f, 1.0f};
        modelRotation    = {0.0f, 0.0f, 0.0f};
    }

    TestTexturedSquare::~TestTexturedSquare()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
        delete texture;
    }

    void TestTexturedSquare::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestTexturedSquare::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);
        shader->Bind();
        //shader.SetUniform4F("u_Color", color);
        Maths::Matrix3D mat = Maths::Matrix3D::Transform(modelTranslation,
                                                         modelScale,
                                                         modelRotation);
        //LOG(mat);
        shader->SetUniform4F("u_Color", color);
        shader->SetUniformMatrix4x4("u_MVP", projection * mat);
        shader->Unbind();
        
        renderer.Draw(*va, *ib, *shader);
    }

    void TestTexturedSquare::OnImGuiRender()
    {
        Test::OnImGuiRender();

        ImGui::ColorEdit4("Texture Color", color);
        ImGui::DragFloat3("Translation", modelTranslation);
        ImGui::DragFloat3("Scale",       modelScale      , 0.1f);
        ImGui::DragFloat3("Rotation",    modelRotation   , 0.03f);
    }
}

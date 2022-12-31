#include "TestBatchedTextured.h"

#include "imgui.h"

namespace Test
{
    TestBatchedTextured::TestBatchedTextured()
    {
        float vertices[(2 + 2 + 1) * 4 * 2] =
        {   -240.0f, -80.0f,    0.0f, 0.0f, 0,
            -80.00f, -80.0f,    1.0f, 0.0f, 0,
            -80.00f, +80.0f,    1.0f, 1.0f, 0,
            -240.0f, +80.0f,    0.0f, 1.0f, 0,
      
            +80.00f, -80.0f,    0.0f, 0.0f, 1,
            +240.0f, -80.0f,    1.0f, 0.0f, 1,
            +240.0f, +80.0f,    1.0f, 1.0f, 1,
            +80.00f, +80.0f,    0.0f, 1.0f, 1,
        };
        
        unsigned int indices[6 * 2] = {
            0, 1, 2,  2, 3, 0,
            4, 5, 6,  6, 7, 4
        };
        
        va = new Graphics::VertexArray();
        vb = new Graphics::VertexBuffer(vertices, (2 + 2 + 1) * 4 * 2 * sizeof(float));

        Graphics::VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        layout.Push<float>(1);
        va->AddBuffer(*vb, layout);

        ib = new Graphics::IndexBuffer(indices, 12);

        projection = Maths::Matrix3D::OrthoProjection(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f);
        //LOG(*mat);
        textures[0] = new Graphics::Texture("src/test/res/TestBatchedTextured/img1.png");
        textures[1] = new Graphics::Texture("src/test/res/TestBatchedTextured/img2.png");
        textures[0]->Bind(0);
        textures[1]->Bind(1);

        shader = new Graphics::Shader("src/test/res/TestBatchedTextured/shader.glsl");
        shader->Bind();
        
        shader->SetUniformMatrix4x4("u_MVP", projection);
        int ids[2] = { 0, 1 };
        shader->SetUniform1I_Arr("u_Texture", ids, 2);

        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        modelTranslation = {0.0f, 0.0f, 0.0f};
        modelScale       = {1.0f, 1.0f, 1.0f};
        modelRotation    = {0.0f, 0.0f, 0.0f};
    }

    TestBatchedTextured::~TestBatchedTextured()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
        delete textures[0];
        delete textures[1];
    }

    void TestBatchedTextured::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestBatchedTextured::OnRender(Graphics::Renderer& renderer)
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

    void TestBatchedTextured::OnImGuiRender()
    {
        Test::OnImGuiRender();
        
        ImGui::DragFloat3("Translation", modelTranslation);
        ImGui::DragFloat3("Scale",       modelScale      , 0.1f);
        ImGui::DragFloat3("Rotation",    modelRotation   , 0.03f);
    }
}

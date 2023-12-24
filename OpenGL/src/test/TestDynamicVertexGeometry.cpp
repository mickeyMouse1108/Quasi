#include "TestDynamicVertexGeometry.h"

#include "imgui.h"

namespace Test
{
    TestDynamicVertexGeometry::TestDynamicVertexGeometry()
     : projection( Maths::Matrix3D::OrthoProjection(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f) )
    {
        vertexes = new VertexColor3D[8]
        {
            { { -240.0f, -80.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { -80.00f, -80.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -80.00f, +80.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
            { { -240.0f, +80.0f, 0.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
            
            { { +80.00f, -80.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
            { { +240.0f, -80.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
            { { +240.0f, +80.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
            { { +80.00f, +80.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
        };
        
        const unsigned int indices[6 * 2] = {
            0, 1, 2,  2, 3, 0,
            4, 5, 6,  6, 7, 4
        };
        
        va = new Graphics::VertexArray();
        vb = new Graphics::DynamicVertexBuffer<VertexColor3D>(8);
        
        va->AddBuffer(*vb);

        ib = new Graphics::IndexBuffer(indices, 12);
        shader = new Graphics::Shader(Graphics::Shader::StdColored);
        shader->Bind();

        shader->SetUniformMatrix4x4("u_MVP", projection);
        
        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();
    }

    TestDynamicVertexGeometry::~TestDynamicVertexGeometry()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
        delete[] vertexes;
    }

    void TestDynamicVertexGeometry::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestDynamicVertexGeometry::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);
        
        vb->SetData(vertexes, 8);
        
        renderer.Draw(*va, *ib, *shader);
    }

    void TestDynamicVertexGeometry::OnImGuiRender()
    {
        Test::OnImGuiRender();

        //constexpr unsigned int size = sizeof(VertexColor3D);
        ImGui::DragFloat3("Red    Vertex [0]", (float*)(vertexes + 0));
        ImGui::DragFloat3("Green  Vertex [1]", (float*)(vertexes + 1));
        ImGui::DragFloat3("Blue   Vertex [2]", (float*)(vertexes + 2));
        ImGui::DragFloat3("Gray   Vertex [3]", (float*)(vertexes + 3));

        ImGui::DragFloat3("Purple Vertex [4]", (float*)(vertexes + 4));
        ImGui::DragFloat3("Yellow Vertex [5]", (float*)(vertexes + 5));
        ImGui::DragFloat3("Cyan   Vertex [6]", (float*)(vertexes + 6));
        ImGui::DragFloat3("White  Vertex [7]", (float*)(vertexes + 7));
    }
}

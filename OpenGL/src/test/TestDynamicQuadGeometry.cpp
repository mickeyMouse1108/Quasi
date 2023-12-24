#include "TestDynamicQuadGeometry.h"
#include "TestDynamicQuadGeometry.h"
#include "TestDynamicQuadGeometry.h"

#include "imgui.h"

namespace Test
{
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
    
    TestDynamicQuadGeometry::TestDynamicQuadGeometry()
    : projection( Maths::Matrix3D::OrthoProjection(-320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f) )
    {
        unsigned int indices[MAX_INDEX];

        for (int i = 0, v = 0; i < MAX_INDEX; i += 6, v += 4)
        {
            indices[i    ] = v;
            indices[i + 1] = v + 1;
            indices[i + 2] = v + 2;
            indices[i + 3] = v + 2;
            indices[i + 4] = v + 3;
            indices[i + 5] = v;
        }       
        
        va = new Graphics::VertexArray();
        vb = new Graphics::DynamicVertexBuffer<VertexColor3D>(MAX_VERTEX);
        
        va->AddBuffer(*vb);

        ib = new Graphics::IndexBuffer(indices, MAX_INDEX);
        shader = new Graphics::Shader(Graphics::Shader::StdColored);
        shader->Bind();

        shader->SetUniformMatrix4x4("u_MVP", projection);
        
        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        for (int i = 4; i < MAX_VERTEX; i++) vertexes[i] = { {0,0,0}, {0,0,0} };

        vertexes[0] = { { -50.0f, -50.0f }, COLORS[0] };
        vertexes[1] = { { +50.0f, -50.0f }, COLORS[0] };
        vertexes[2] = { { +50.0f, +50.0f }, COLORS[0] };
        vertexes[3] = { { -50.0f, +50.0f }, COLORS[0] };
        quads = 1;
    }

    TestDynamicQuadGeometry::~TestDynamicQuadGeometry()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
    }

    void TestDynamicQuadGeometry::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestDynamicQuadGeometry::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);
        
        vb->SetData(vertexes, quads * 4);
        renderer.Draw(*va, *ib, *shader);
    }

    void TestDynamicQuadGeometry::OnImGuiRender()
    {
        Test::OnImGuiRender();

        ImGui::DragFloat3("Quad Vertex 1", (float*)(vertexes + quads * 4 - 4));
        ImGui::DragFloat3("Quad Vertex 2", (float*)(vertexes + quads * 4 - 3));
        ImGui::DragFloat3("Quad Vertex 3", (float*)(vertexes + quads * 4 - 2));
        ImGui::DragFloat3("Quad Vertex 4", (float*)(vertexes + quads * 4 - 1));

        if (ImGui::Button("Add Quad") && !isMax)
        {
            if (quads >= MAX_QUAD)
            {
                isMax = true;
            } else
            {
                vertexes[quads * 4    ] = { { -50.0f, -50.0f }, COLORS[quads] };
                vertexes[quads * 4 + 1] = { { +50.0f, -50.0f }, COLORS[quads] };
                vertexes[quads * 4 + 2] = { { +50.0f, +50.0f }, COLORS[quads] };
                vertexes[quads * 4 + 3] = { { -50.0f, +50.0f }, COLORS[quads] };
                quads++;

                isMin = false;
            }
        }
        
        if (isMax) ImGui::Text("Too Many Quads! (Limit: %i)", MAX_QUAD); 
        
        if (ImGui::Button("Remove Quad") && !isMin)
        {
            if (quads <= 1) { isMin = true; } else
            {
                vertexes[quads * 4 - 4] = { { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
                vertexes[quads * 4 - 3] = { { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
                vertexes[quads * 4 - 2] = { { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } }; 
                vertexes[quads * 4 - 1] = { { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
                quads--;

                isMax = false;

                vb->SetData(vertexes, (quads + 1) * 4);
            }
        }

        if (isMin) ImGui::Text("Can't Delete All Quads!");
    }
}
